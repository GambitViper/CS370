#include <stdio.h>
#include <stdlib.h>

void HexToBinary(int *, unsigned int);
void ReverseArray(int *, int);
void ReverseArrayHelper(int *, int, int);
void Dissassemble(int *);

int main() {

    FILE *fp;
    unsigned int hex;

    fp = fopen("trace.txt", "r");
    while (fscanf(fp, "%X", &hex) != EOF){
        //PC address bits
        printf("PC~~~");
        int *pc_bits;
        pc_bits = (int *) malloc(32 * sizeof(int));
        HexToBinary(pc_bits, hex);
        ReverseArray(pc_bits, 32);
        for(int i = 0; i < 32; ++i){
            printf("%d", pc_bits[i]);
        }

        //Instruction MIPS encoded bits
        printf("\n~~~INSTR~~~");
        int *instr_bits;
        instr_bits = (int *) malloc(32 * sizeof(int));
        fscanf(fp, "%X", &hex);
        HexToBinary(instr_bits, hex);
        ReverseArray(instr_bits, 32);
        for(int i = 0; i < 32; ++i){
            printf("%d", instr_bits[i]);
        }
        //Dissassemble(instr_bits);
        printf("\n");
    }

    return 0;
}

void Dissassemble(int *bits){
  //char opcode[];
  //for(int )
}

void HexToBinary(int *bits, unsigned int hex){
    for(int i = 0; i < 32; ++i){
        bits[i] = (hex >> i) & 1;
    }
}

void ReverseArray(int *arr, int len){
  int start = 0, end = len - 1;
  ReverseArrayHelper(arr, start, end);
}

void ReverseArrayHelper(int *arr, int start, int end){
  int temp;
  if(start >= end) return;
  temp = arr[start];
  arr[start] = arr[end];
  arr[end] = temp;
  ReverseArrayHelper(arr, start + 1, end - 1);
}
