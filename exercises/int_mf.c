#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, const char *argv[])
{
    int *ptr;
    ptr = (int *)malloc(sizeof(int));
    scanf("%d", ptr);
    printf("%d\n", *ptr);
    free(ptr);
    return 0;
}
