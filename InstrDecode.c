#include <stdio.h>
#include <stdlib.h>

void HexToBinary(int *, unsigned int);
void ReverseArray(int *, int);
void ReverseArrayHelper(int *, int, int);
unsigned int GetHexValue(unsigned int, int, int);
struct statistics{
  int insts;
  int rtype;
  int itype;
  int jtype;
  int fwdtaken;
  int bkwtaken;
  int nottaken;
  int loads;
  int stores;
  int arith;

  int regreads [32];
  int regwrites [32];
};
typedef struct statistics statistics;
void Dissassemble(unsigned int, statistics *);
statistics * Statistics();

int main() {

    FILE *fp;
    unsigned int pcaddr;
    unsigned int einstr;

    statistics *stats;
    stats = Statistics();
    // int *pc_bits;
    // int *instr_bits;
    int i, j;

    fp = fopen("trace.txt", "r");
    while (fscanf(fp, "%X %X", &pcaddr, &einstr) != EOF){
        //PC address bits
        printf("~~~~~PC~~~~");
        printf("%X\n", pcaddr);
        // pc_bits = (int *) malloc(32 * sizeof(int));
        // if(pc_bits == NULL){
        //   printf("pc_bits null\n");
        // }
        // HexToBinary(pc_bits, pcaddr);
        // ReverseArray(pc_bits, 32);

        // for(i = 0; i < 32; ++i){
        //     printf("%d", pc_bits[i]);
        // }

        //Instruction MIPS encoded bits
        printf("~~~INSTR~~~");
        printf("%X\n", einstr);
        // instr_bits = (int *) malloc(32 * sizeof(int));
        // if(instr_bits == NULL){
        //   printf("inner_bits null\n");
        // }
        // HexToBinary(instr_bits, einstr);
        // ReverseArray(instr_bits, 32);
        //
        // for(j = 0; j < 32; ++j){
        //     printf("%d", instr_bits[j]);
        // }
        Dissassemble(einstr, stats);

        printf("\n");
    }
    free(stats);
    fclose(fp);

    return 0;
}

statistics * Statistics(){
  statistics *newstat = (statistics *) malloc(sizeof(statistics));
  if(newstat == NULL){
    return NULL;
  }

  newstat->insts = 0;
  newstat->rtype = 0;
  newstat->itype = 0;
  newstat->jtype = 0;
  newstat->fwdtaken = 0;
  newstat->bkwtaken = 0;
  newstat->nottaken = 0;
  newstat->loads = 0;
  newstat->stores = 0;
  newstat->arith = 0;

  int i;
  for(i = 0; i < 32; ++i){
    newstat->regreads[i] = 0;
    newstat->regwrites[i] = 0;
  }

  return newstat;
}

void Dissassemble(unsigned int encoded, statistics *stats){
  // printf("~~~DIS~~~\n");
  unsigned int opcode, rs, rt, rd, shamt, funct, imm, addr;
  opcode = GetHexValue(encoded, 31, 26);
  rs = GetHexValue(encoded, 25, 21);
  rt = GetHexValue(encoded, 20, 16);
  rd = GetHexValue(encoded, 15, 11);
  shamt = GetHexValue(encoded, 10, 6);
  funct = GetHexValue(encoded, 5, 0);
  // imm = GetHexValue(bits, 16, 31, 16);
  // addr = GetHexValue(bits, , 25, 26);
  printf("\n\topcode--> %d\n", opcode);
  printf("\n\trs--> %d\n", rs);
  printf("\n\trt--> %d\n", rt);
  printf("\n\trd--> %d\n", rd);
  printf("\n\tshamt--> %d\n", shamt);
  printf("\n\tfunct--> %d\n", funct);
  // printf("\n\timm--> %d\n", imm);
  // printf("\n\taddr--> %d\n", addr);
}

unsigned int GetHexValue(unsigned int encoded, int hi, int lo){
  // printf("~~~GXV~~~\n");
  unsigned int range = (hi - lo + 1);
  if(range == 32) return encoded;
  unsigned int result = 0;
  unsigned int mask = ((1 << range) -1) << lo;
  result = encoded & mask;
  result = result >> lo;
  return result;
}

void HexToBinary(int *bits, unsigned int hex){
    int i;
    for(i = 0; i < 32; ++i){
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
