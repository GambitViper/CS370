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
void PrintStatistics(statistics *);
statistics * Statistics();

// DEBUG bit
int debug = 1;

int main() {

    FILE *fp;
    unsigned int pcaddr;
    unsigned int einstr;

    statistics *stats;
    stats = Statistics();

    fp = fopen("trace.txt", "r");
    while (fscanf(fp, "%X %X", &pcaddr, &einstr) != EOF){

        // PC address bits
        if(debug == 1){
          printf("~~~~~PC~~~~");
          printf("%X\n", pcaddr);
        }

        // Instruction MIPS encoded bits
        if(debug == 1){
          printf("~~~INSTR~~~");
          printf("%X\n", einstr);
        }

        // Main function dissasembles instructions
        // & Records to statistics struct
        Dissassemble(einstr, stats);

        if(debug == 1){
          printf("\n");
        }
    }

    PrintStatistics(stats);

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
  unsigned int opcode, rs, rt, rd, shamt, funct, imm, addr;
  opcode = GetHexValue(encoded, 31, 26);
  rs = GetHexValue(encoded, 25, 21);
  rt = GetHexValue(encoded, 20, 16);
  rd = GetHexValue(encoded, 15, 11);
  shamt = GetHexValue(encoded, 10, 6);
  funct = GetHexValue(encoded, 5, 0);

  if(debug == 1){
    printf("\topcode--> %d\n", opcode);
    printf("\trs--> %d\n", rs);
    printf("\trt--> %d\n", rt);
    printf("\trd--> %d\n", rd);
    printf("\tshamt--> %d\n", shamt);
    printf("\tfunct--> %d\n", funct);
  }

  stats->insts++;
  
  if(opcode == 0){
    // R-type Instructions
    stats->rtype++;
    if(debug == 1) printf("r-type\n");
    if((funct >= 0x20 && funct <= 0x24) 
        || funct == 0x27 
        || (funct >= 0x2a && funct <= 0x2b)){
      // R[rd] = R[rs] & R[rt]
      if(debug == 1) printf("\tR[%d] = R[%d] & R[%d]", rd, rs, rt);
      stats->regwrites[rd]++;
      stats->regreads[rs]++;
      stats->regreads[rt]++;
    }else if(funct == 0x0 || (funct >= 0x2 && funct <= 0x3)){
      // R[rd] = R[rt]
      if(debug == 1) printf("\tR[%d] = R[%d]", rd, rt);
      stats->regwrites[rd]++;
      stats->regreads[rt]++;
    }else if(funct == 0x8){
      // PC = R[rs]
      if(debug == 1) printf("\tPC = R[%d]", rs);
      stats->regreads[rs]++;
    }
  }else if(opcode <= 3){
    // J-type Instructions
    stats->jtype++;
    addr = GetHexValue(encoded, 25, 0);
    if(debug == 1){
      printf("j-type\n");
      printf("\taddr--> %d\n", addr);
    }
  }else{
    // I-type Instructions
    stats->itype++;
    imm = GetHexValue(encoded, 15, 0);
    if(debug == 1){
      printf("i-type\n");
      printf("\timm--> %d\n", imm);
    }
    if((funct >= 0xa && funct <= 0xd) 
        || (funct >= 0x23 && funct <= 0x25) 
        || (funct >= 0x8 && funct <= 0x9)){
      // R[rt] = R[rs]
      if(debug == 1) printf("\tR[%d] = R[%d] ...", rt, rs);
      stats->regwrites[rt]++;
      stats->regreads[rs]++;
    }
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

void PrintStatistics(statistics *stats){
  printf("\n~~~~~~~~~~~~~~~~~~~~~\n");
  printf("insts: %d\n", stats->insts);
  printf("r-type: %d\n", stats->rtype);
  printf("i-type: %d\n", stats->itype);
  printf("j-type: %d\n", stats->jtype);
  printf("fwd-taken: %d\n", stats->fwdtaken);
  printf("bkw-taken: %d\n", stats->bkwtaken);
  printf("not-taken: %d\n", stats->nottaken);
  printf("loads: %d\n", stats->loads);
  printf("stores: %d\n", stats->stores);
  printf("arith: %d\n", stats->arith);
  int i;
  for(i = 0; i < 32; ++i){
    printf("reg-%d: %d %d\n", i, stats->regreads[i], stats->regwrites[i]);
  }
}
