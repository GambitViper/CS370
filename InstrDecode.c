#include <stdio.h>
#include <stdlib.h>

void HexToBinary(int *, unsigned int);

int main() {

    FILE *fp;
    char line[255];

    fp = fopen("trace.txt", "r");
    while (fscanf(fp, "%s", line) != EOF){
        unsigned int num = (unsigned int) strtol(line, NULL, 16);
        int *bits;
        bits = (int *) malloc(32 * sizeof(int));
        HexToBinary(bits, num);
        for(int i = 0; i < 32; ++i){
            printf("%d", bits[i]);
        }
        printf("\n");
    }
    
    return 0;
}

void HexToBinary(int *bits, unsigned int hex){
    for(int i = 0; i < 32; ++i){
        bits[i] = (hex >> i) & 1;
    }
}
