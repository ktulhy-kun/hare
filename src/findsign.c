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
            if (memcmp(buffer + i, str, 4) == 0){
                //printf("%lx\n", num + i + pos);
                fseek(f, num + i + pos, SEEK_SET);
                return num + i + pos;
            }
        }
        if (num_moved == 0) num_moved = 3;
        memmove(buffer, buffer + SIZE_BUF - num_moved, num_moved);
        num += SIZE_BUF - num_moved;
    }
    return 0;
}
