#include <stdio.h>

#define DELTA ('a' - 'A')

char toggle(char c)
{
    if('A' <= c && c <= 'Z')
        return c + DELTA;
    else if('a' <= c && c <= 'z')
        return c - DELTA;
    else
        return c;
}

int main(int argc, const char *argv[])
{
    char buf[64];
    scanf("%s", buf);
    char *current = buf;
    do {
        *current = toggle(*current);
        current++;
    } while (*current);
    printf("%s\n", buf);
    return 0;
}
