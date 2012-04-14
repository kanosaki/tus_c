#include <stdio.h>

void
string_copy(char *src, char *dst)
{
    char *current = src;
    while(*current){
        *dst = *current;
        dst++;
        current++;
    }
    *dst = 0;
}


int 
main(int argc, const char *argv[])
{
    char first[] = "first";
    char second[20];
    string_copy(first, second);
    printf("%s\n", second);
    return 0;
}
