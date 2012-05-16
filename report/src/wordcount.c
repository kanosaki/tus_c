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

int
word_equals(word* left, word* right){
    return ! strcmp(left->str, right->str);
}

word*
word_create(char* value){
    word * ret = (word *)malloc(sizeof(word));
    ret->str = value;
    ret->count = 0;
    ret->next = NULL;
    return ret;
}

typedef struct TABLE_T {
    word **table;
    size_t table_length;
} word_table;


word_table*
wt_create(size_t capacity){
    word_table* table = (word_table *)malloc(sizeof(word_table));
    table->table_length = capacity;
    table->table = (word **)calloc(capacity, sizeof(word));
    return table;
}

static inline int
wt_row_number(word_table* table, char* value){
    return word_hash(value) % table->table_length;
}

static inline word*
wt_find_row(word_table* table, char* value){
    int row_number = wt_row_number(table, value);
    return table->table[row_number];
}

static word*
wt_row_seek(word* current, char* value){
    if (!current) return NULL; // not found.

    if(!strcmp(current->str, value)) // Match
        return current;
    else // not match
        return wt_row_seek(current->next, value);
}

static void
wt_row_push(word* current, word* value){
    if(!current->next)
        current->next = value;
    else
        wt_row_push(current->next, value);
}

static void
wt_new_row(word_table* table, word* wd){
    int row_number = wt_row_number(table, wd->str);
    table->table[row_number] = wd;
}

word*
wt_find(word_table* table, char* value){
    word* row_root = wt_find_row(table, value);
    return wt_row_seek(row_root, value);
}

void
wt_push(word_table* table, char* value){
    word *wd = word_create(value);
    word *row_root = wt_find_row(table, value);
    if(!row_root){
        wt_new_row(table, wd);
    } else {
        wt_row_push(row_root, wd);
    }
}


int main(int argc, const char *argv[])
{
    return 0;
}
