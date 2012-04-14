
// 問題の意味がよくわからない

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* string_duplicate(char* src)
{
    char* buf = (char *)malloc(sizeof(char) * strlen(src));
    strcpy(buf, src);
    return buf;
}

int 
main(int argc, const char *argv[])
{
    char src[] = "foobar";
    char* dup = string_duplicate(src);
    printf("%s\n", dup);
    free(dup);
    return 0;
}
