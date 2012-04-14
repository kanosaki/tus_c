
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char data[30][100];
int data_index;

int main(int argc, const char *argv[])
{
    char buf[1024];
    int i;
    data_index = 0;
    while (scanf("%s", buf) != EOF) {
        if (strcmp("end", buf) == 0)
            break;
        strcpy(data[data_index], buf);
        data_index += 1;
    }

    printf("-------\n");
    for (i = 0; i < data_index; i++) {
        printf("%s\n", data[i]);
    }
    return 0;
}
