#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BIT_FILLED 0xFFFFFFFFUL
#define POLY 0xEDB88320UL

typedef uint8_t byte;
typedef uint32_t crc_t;

uint64_t crc_table[0xFF];
bool crc_table_is_inited = false;

void
gen_crc_table(void){
    crc_t crc;
    int i,j;

    for(i = 0; i < 256; i++){
        crc = i;
        for(j = 8; j > 0; j--){
            if(crc & 1){
                crc = (crc >> 1) ^ POLY;
            }else{
                crc >>= 1;
            }
        }
        crc_table[i] = crc;
    }
    crc_table_is_inited = true;
}

crc_t
crc_bytes(byte* data, size_t len){
    if(!crc_table_is_inited){
        gen_crc_table();
    }
    crc_t crc;
    size_t i;
    crc = BIT_FILLED;
    for(i = 0; i < len; i++){
        crc = ((crc >> 8) & 0x00FFFFFF) ^ crc_table[(crc ^ *data++) & 0xFF];
    }
    return (crc ^ BIT_FILLED);
}

crc_t
crc_string(char* data){
    if(!crc_table_is_inited){
        gen_crc_table();
    }
    crc_t crc;
    for(crc = BIT_FILLED; *data;)
        crc = ((crc >> 8) & 0x00FFFFFF) ^ crc_table[(crc ^ *data++) & 0xFF];
    return (crc ^ BIT_FILLED);
}
/* 
crc_t
crc_native(byte* data, size_t len){
    uint32_t i,j;
    crc_t r;

    r = BIT_FILLED;
    for(i = 0; i < len; i++){
        r ^= *data++;
        for(j = 0; j < 8; j++){
            if(r & 1){
                r = (r >> 1) ^ POLY;
            }else{
                r >>= 1;
            }
        }
    }
    return r ^ BIT_FILLED;
}

size_t
get_filesize(FILE *fp){
    long origin = ftell(fp);
    size_t size = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, origin, SEEK_SET);
    return size;
}

void
print_crc(char* filename){
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        printf("Cannot open file.\n");
        return;
    }
    size_t size = get_filesize(fp);
    byte* buffer = (byte*)malloc(size);
    if(buffer == NULL){
        printf("Memory allocation Failed.\n");
        return;
    }
    fread(buffer, sizeof(byte), size, fp);
    
    crc_t result = crc_native(buffer, size);
    free(buffer);
    printf("%x\n",(uint32_t)result);
}

int
main(int argc, char** argv){
    if(argc > 0){
        print_crc(argv[1]);
    }else{
        printf("File name is not givenn\n");
    }
    return 0;
}
*/
