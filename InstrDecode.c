#include <stdio.h>
#include <stdlib.h>

void HexToBinary(int *, unsigned int);
void ReverseArray(int []);
void ReverseArrayHelper(int [], int, int);

int main() {

    FILE *fp;
    unsigned int hex;

    fp = fopen("trace.txt", "r");
    while (fscanf(fp, "%X", &hex) != EOF){
        int *bits;
        bits = (int *) malloc(32 * sizeof(int));
        HexToBinary(bits, hex);
        ReverseArray(bits);
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

void ReverseArray(int *bits){
  int len = sizeof(&bits) / sizeof(int);
  int start = 0, end = len - 1;
  ReverseArrayHelper(bits, start, end);
}

void ReverseArrayHelper(int *arr, int start, int end){
  int temp;
  if(start >= end) return;
  temp = arr[start];
  arr[start] = arr[end];
  arr[end] = temp;
  ReverseArrayHelper(arr, start + 1, end - 1);
}
