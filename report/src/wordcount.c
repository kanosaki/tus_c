#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crc.c"

/**
 *  'wt' is abbreviation of 'word table'
 *
 *  word table structure
 *  word_table(container)
 *      ->table (array of )
 */

typedef struct WORD_T {
    char *str;
    int count;
    struct WORD_T *next;
} word;

int
word_hash(char* wd){
    return crc_string(wd);
}

// 新しいword構造体を構成します
word*
word_create(char* value){
    word * ret = (word *)malloc(sizeof(word));
    ret->str = value;
    ret->count = 0;
    ret->next = NULL;
    return ret;
}

// ハッシュテーブル本体の定義
typedef struct TABLE_T {
    word **table;
    size_t table_length;
} word_table;


// 指定された容量を持つハッシュテーブルを新規に作成
word_table*
wt_create(size_t capacity){
    word_table* table = (word_table *)malloc(sizeof(word_table));
    table->table_length = capacity;
    table->table = (word **)calloc(capacity, sizeof(word));
    return table;
}

// 文字列のテーブルにおける実際の配列の添え字を計算します
static inline int
wt_row_number(word_table* table, char* value){
    return word_hash(value) % table->table_length;
}

// 渡された文字列を持つかも知れない列の先頭のword構造体を返します。
// 存在しない場合はNULLが返されます
static inline word*
wt_find_row(word_table* table, char* value){
    int row_number = wt_row_number(table, value);
    return table->table[row_number];
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

// テーブルに新しく文字列をもつword構造体を構成し、追加します。
void
wt_push(word_table* table, char* value){
    word *wd = word_create(value);
    word *row_root = wt_find_row(table, value);
    if(!row_root)
        wt_row_new(table, wd);
    else 
        wt_row_push(row_root, wd);
}

void
register_word(word_table* table, char* value){
    word* prev = wt_find(table, value);
    if(prev) // Already exists
        prev->count += 1;
    else // First appeared
        wt_push(table, value);
}

int
main(int argc, const char *argv[])
{
    return 0;
}

