
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, const char *argv[])
{
    char buf[1024];
    int i;
    int data_index, data_len;
    char *data[data_len];
    scanf("%d", &data_len);
    data_index = 0;
    while (scanf("%s", buf) != EOF) {
        if (strcmp("end", buf) == 0)
            break;
        char* body = (char *)malloc(sizeof(char) * strlen(buf));
        strcpy(body, buf);
        data[data_index] = body;
        data_index += 1;
    }

    printf("-------\n");
    for (i = 0; i < data_index; i++) {
        printf("%s\n", data[i]);
        free(data[i]);
    }
    return 0;
}
