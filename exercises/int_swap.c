#include <stdio.h>

void
swap(int *src, int *dst)
{
    int tmp = *dst;
    *dst = *src;
    *src = tmp;
}


int 
main(int argc, const char *argv[])
{
    int a = 100, b = 200;
    printf("before: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("after:  a = %d, b = %d\n", a, b);
    return 0;
}
