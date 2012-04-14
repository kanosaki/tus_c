#include <stdio.h>

int string_length(char *c)
{
    int ret = 0;
    while(*c){
        ret++;
        c++;
    }
    return ret;
}

int main(int argc, const char *argv[])
{
    char buf[30];
    scanf("%s", buf);
    printf("%d\n", string_length(buf));
    return 0;
}
