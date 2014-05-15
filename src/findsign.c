#include "findsign.h"

int findSignature(FILE *f){
    long int pos = ftell(f);
    char *str = "\x07\x1F\x0E\x58";
    char *buffer = (char *)calloc(SIZE_BUF, sizeof(char));
    int num_moved = 0;
    int i, num = 0;
    while (!feof(f)){
        fread(buffer + num_moved, sizeof(char), SIZE_BUF - num_moved, f);
        for (i = 0; i < SIZE_BUF;i++) {
            if (strncmp(buffer + i, str, 4) == 0){
                printf("%i\n", num + i + 1);
                fseek(f, num + i + pos, SEEK_SET);
                memset(buffer, 0, 15);
                fread(buffer, sizeof(char), SIZE_BUF, f);
                printf("%s", buffer);
                return 0;
            }
        }
        if (num_moved == 0) num_moved = 3;
        memmove(buffer, buffer + SIZE_BUF - num_moved, num_moved);
        num += SIZE_BUF - num_moved;
    }
    return 0;
}
