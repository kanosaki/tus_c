#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char buf[1024];
    while(scanf("%s", buf) != EOF){
        char* alloced = (char *)malloc(sizeof(char) * strlen(buf));
        strcpy(alloced, buf);
        printf("%s\n", alloced);
        free(alloced);
    }
    return 0;
}
