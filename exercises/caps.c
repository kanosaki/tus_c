#include <stdio.h>

#define DELTA ('a' - 'A')

int main(int argc, const char *argv[])
{
    char src[] = "abcdefg";
    char *current = src;
    printf("%s\n", src);
    do {
        *current -= DELTA;
        current++;
        printf("%s\n", src);
    } while (*current);
    return 0;
}
