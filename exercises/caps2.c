#include <stdio.h>

#define DELTA ('a' - 'A')

char upper(char c)
{
    return c - DELTA;
}

int main(int argc, const char *argv[])
{
    char buf[64];
    scanf("%s", buf);
    char *current = buf;
    printf("%s\n", buf);
    do {
        *current = upper(*current);
        current++;
        printf("%s\n", buf);
    } while (*current);
    return 0;
}
