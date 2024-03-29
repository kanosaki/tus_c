#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "crc.c"

#define MAX_WORD_LENGTH 128
#define TOKENIZED_SCAN 1
/*
 * Avaivable hash functions:
 *  crc_string
 *  hash_simple_sum
 *  hash_simple_product
 */
#define DEFAULT_HASH_FUNCTION hash_crc

#define WT_FOREACH(table, elem, statements) do {\
        int _row_num;\
        for(_row_num = 0; _row_num < table->table_length; _row_num++){\
            word* elem = wt_row(table, _row_num);\
            while(elem){\
                statements;\
                elem = elem->next;\
            }\
        }\
    }while(0)



/**
 *  'wt' is abbreviation of 'word table'
 *
 *  word table structure
 *  word_table(container)
 *      ->table (array of )
 */

// Mecellaneous functions/*{{{*/
void
print_usage(){
    printf("Usage: wordcount table_length file\n");
}

void
exit_error(char *msg){
    fprintf(stderr, "%s\n", msg);
    exit(-1);
}
/*}}}*/
// Statistics functions/*{{{*/

static double
int_list_summation(int* values, int len){
    int i = 0;
    double ret = 0.0;
    for(;i < len; i++){
        ret += values[i];
    }
    return ret;
}

static double
int_list_unbiased_variance(int* values, int len, double average){
    int i = 0;
    double delta_sum = 0.0;
    for(; i < len; i++){
        delta_sum += pow(average - values[i], 2);
    }
    return delta_sum / (len - 1);
}
/*}}}*/
/* word functions {{{*/
typedef struct WORD_T {
    char *str;
    int count;
    struct WORD_T *next;
} word;

static int
hash_crc(char* wd){
    return crc_string(wd);
}

static int
hash_simple_sum(char* wd){
    int ret;
    for(ret = 0; *wd; wd++) ret += *wd;
    return ret;
}

static int
hash_simple_product(char* wd){
    int ret;
    for(ret = 1; *wd; wd++) ret *= *wd;
    return ret;
}

// 新しいword構造体を構成します
word*
word_create(char* value){
    word * ret = (word *)malloc(sizeof(word));
    ret->str = (char*)malloc(strlen(value));
    strcpy(ret->str, value);
    ret->count = 1;
    ret->next = NULL;
    return ret;
}

void
word_free(word* wd){
    word* next = wd->next;
    free(wd->str);
    if(next) word_free(next);
}
/*}}}*/
/* word_table functions {{{*/
// ハッシュテーブル本体の定義
typedef struct TABLE_T {
    word **table;
    size_t table_length;
    size_t items_count;
    int (*hash_function)(char*);
} word_table;


// 指定された容量を持つハッシュテーブルを新規に作成
word_table*
wt_create(size_t capacity){
    word_table* table = (word_table *)malloc(sizeof(word_table));
    table->table_length = capacity;
    table->items_count = 0;
    table->table = (word **)calloc(capacity, sizeof(word));
    table->hash_function = &DEFAULT_HASH_FUNCTION;
    return table;
}

static inline word*
wt_row(word_table* table, int row_num){
    return table->table[row_num];
}

// 文字列のテーブルにおける実際の配列の添え字を計算します
static inline int
wt_row_number(word_table* table, char* value){
    return table->hash_function(value) % table->table_length;
}

static int
wt_row_length(word* row){
    int ret;
    for(ret = 0; row; row = row->next) ret++;
    return ret;
}

// 渡された文字列を持つかも知れない列の先頭のword構造体を返します。
// 存在しない場合はNULLが返されます
static inline word*
wt_find_row(word_table* table, char* value){
    int row_number = wt_row_number(table, value);
    return wt_row(table, row_number);
}

// 指定された文字列を持つかもしれない列を線形的に検索します。
// 見つからなかった場合はNULLが返ります。
static word*
wt_row_seek(word* current, char* value){
    if (!current) return NULL; // not found.

    if(!strcmp(current->str, value)) // Match
        return current;
    else // not match
        return wt_row_seek(current->next, value);
}

// 新しいword構造体を列に追加します。この関数は追加のみで、
// 新規に列を作る場合はwt_row_new関数を使ってください。
static void
wt_row_push(word* current, word* value){
    if(!current->next)
        current->next = value;
    else
        wt_row_push(current->next, value);
}

// 新規に列を作成します
static void
wt_row_new(word_table* table, word* wd){
    int row_number = wt_row_number(table, wd->str);
    table->table[row_number] = wd;
}

// テーブルから指定された文字列を持つword構造体を検索します。
// 見つからない場合はNULLを返します。
word*
wt_find(word_table* table, char* value){
    word* row_root = wt_find_row(table, value);
    return wt_row_seek(row_root, value);
}

static void
wt_push_word(word_table* table, word* wd){
    word *row_root = wt_find_row(table, wd->str);
    if(!row_root)
        wt_row_new(table, wd);
    else 
        wt_row_push(row_root, wd);
    table->items_count += 1;
}

// テーブルに新しく文字列をもつword構造体を構成し、追加します。
word*
wt_push(word_table* table, char* value){
    word *prev = wt_find(table, value);
    if(prev) return prev;
    word *wd = word_create(value);
    wt_push_word(table, wd);
    return wd;
}

static void
wt_row_summary(word_table* table, int* result){
    int row_num = 0;
    for(; row_num < table->table_length; row_num++){
        result[row_num] = wt_row_length(wt_row(table, row_num));
    }
}

static void
wt_print_table_length_summary(int* table, int len, int column){
    int i = 0, col = 0;
    for(; i < len; i++){
        printf("[%4d]: %4d| ", i, table[i]);
        col++;
        if(col == column){
            printf("\n");
            col = 0;
        }
    }
    if(col) printf("\n");
}

void
wt_print_bias(word_table* table){
    int len = table->table_length;
    int len_table[len];
    double average = 0.0, variance = 0.0;
    wt_row_summary(table, len_table);
    wt_print_table_length_summary(len_table, len, 4);
    average = int_list_summation(len_table, len) / len;
    variance = int_list_unbiased_variance(len_table, len, average);
    printf("Table Count: %d, Average: %f, Variance: %f SD: %f \n", 
            len, average, variance, sqrt(variance));
}

static void
wt_print_row(word* wd){
    int col = 0;
    while(wd){
        printf("'%-15.15s'@%4d ", wd->str, wd->count);
        col++;
        if(col == 4){ printf("\n"); col = 0; }
        wd = wd->next;
    }
}

void
wt_print_all(word_table* table){
    int row_num;
    for(row_num = 0; row_num < table->table_length; row_num++){
        printf("Row: [%d]\n", row_num);
        wt_print_row(wt_row(table, row_num));
        printf("\n");
    }
}

void
wt_free(word_table* table){
    int i;
    for (i = 0; i < table->table_length; i++) {
        word_free(wt_row(table, i));
    }
    free(table);
}

static void
wt_copy(word_table* src, word_table* dst){
    WT_FOREACH(src, wd, wt_push_word(dst, wd));
}

word_table*
wt_rebuild(word_table* src, size_t newsize){
    word_table* newtable = wt_create(newsize);
    wt_copy(src, newtable);
    wt_free(src);
    return newtable;
}
/*}}}*/
// Input tokenizer/*{{{*/

bool word_char_table[0xFF];
bool is_word_char_inited = false;
int special_word_chars[] = { '-', '_' };

static void
init_word_char_table(){
    // Numbers
    int c;
    for(c = '0'; c <= '9'; c++)
        word_char_table[c] = true;
    for(c = 'a'; c <= 'z'; c++)
        word_char_table[c] = true;
    for(c = 'A'; c <= 'Z'; c++)
        word_char_table[c] = true;
    int i;
    for(i = 0; i < sizeof(special_word_chars) / sizeof(char); i++){
        word_char_table[special_word_chars[i]] = true;
    }
}

static inline bool
is_word_char(char c){
    return word_char_table[(int)c];
}

int
scan_token(FILE* fp, char* out, size_t limit){
    if (!is_word_char_inited) init_word_char_table();
    size_t read = 0;
    char c;
    bool is_overflowed = false;
    while(1){
        if(read > limit - 1 && !is_overflowed){
            fprintf(stderr, "WARNING: Token buffer overflow\n");
            is_overflowed = true;
        }
        if((c = fgetc(fp)) != EOF && is_word_char(c)){
            if(!is_overflowed){
                *out = c;
                read++;
                out++;
            }
        }
        else
            break;
    }
    *out = 0;
    while((c = fgetc(fp)) != EOF){
        if(is_word_char(c)){
            ungetc(c, fp);
            break;
        }
    }
    return read;
}
/*}}}*/
// Input file handers/*{{{*/
void
register_word(word_table* table, char* value){
    word* prev = wt_find(table, value);
    if(prev) // Already exists
        prev->count += 1;
    else // First appeared
        wt_push(table, value);
}


void
aggregate_words(FILE* fp, word_table* table){
    char buffer[MAX_WORD_LENGTH];
#if TOKENIZED_SCAN
    while(scan_token(fp, buffer, MAX_WORD_LENGTH)){
#else
    while(fscanf(fp, "%s", buffer) != EOF){
#endif
        register_word(table, buffer);
    }
}

void 
read_file(const char* filename, word_table *table){
    FILE *fp = fopen(filename, "r");
    if(!fp) exit_error("Cannot open file");
    aggregate_words(fp, table);
    fclose(fp);
}

void 
read_print_loop(word_table* table){
    char buffer[MAX_WORD_LENGTH];
    printf("Word > ");
    while(scanf("%s", buffer) != EOF){
        word* wd = wt_find(table, buffer);
        if(wd){
            printf("%s: %d\n", wd->str, wd->count);
        } else {
            printf("'%s' was not found\n", buffer);
        }
        printf("Word > ");
    }
}

/*}}}*/
int
main(int argc, const char *argv[])
{
    if(argc < 3){
        print_usage();
        exit(1);
    }
    size_t capacity = atoi(argv[1]);
    word_table* table = wt_create(capacity);

    read_file(argv[2], table);
    // wt_print_all(table);
    wt_print_bias(table);
    printf("All words: %zd\n", table->items_count);
    read_print_loop(table);
    wt_free(table);
    return 0;
}

