#include <stdio.h>
#include <stdlib.h>

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
  // 
  // addr = GetHexValue(bits, , 25, 26);
  printf("\topcode--> %d\n", opcode);
  printf("\trs--> %d\n", rs);
  printf("\trt--> %d\n", rt);
  printf("\trd--> %d\n", rd);
  printf("\tshamt--> %d\n", shamt);
  printf("\tfunct--> %d\n", funct);
  
  if(opcode == 0){
    printf("r-type\n");
    if((funct >= 0x20 && funct <= 0x24) || funct == 0x27 || (funct >= 0x2a && funct <= 0x2b)){
      printf("\t~write -> R[%d], read -> R[%d] & R[%d]", rd, rs, rt);
    }else if(funct == 0x0 || (funct >= 0x2 && funct <= 0x3)){
      printf("\t~write -> R[%d], read -> R[%d]", rd, rt);
    }else if(funct == 0x8){
      printf("\twrite -> PC, read -> R[%d]", rs);
    }
  }else if(opcode <= 3){
    printf("j-type\n");
    addr = GetHexValue(encoded, 25, 0);
    printf("\taddr--> %d\n", addr);

  }else{
    printf("i-type\n");
    imm = GetHexValue(encoded, 15, 0);
    printf("\timm--> %d\n", imm);
  }
}

unsigned int GetHexValue(unsigned int encoded, int hi, int lo){
  unsigned int range = (hi - lo + 1);
  if(range == 32) return encoded;
  unsigned int result = 0;
  unsigned int mask = ((1 << range) -1) << lo;
  result = encoded & mask;
  result = result >> lo;
  return result;
}
