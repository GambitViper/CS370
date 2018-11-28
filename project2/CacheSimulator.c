#include <stdio.h>
#include <stdlib.h>

unsigned int GetHexValue(unsigned int, int, int);
struct statistics{
    int rhits;
    int whits;
    int rmisses;
    int wmisses;
    int wb;
    int wt;
};
typedef struct statistics statistics;
struct cache_set{
    int *valid;
    int *dirty;
    unsigned int *tag;
    int blocktoreplace;
};
typedef struct cache_set cache_set;
void PrintStatistics(statistics *);
statistics * Statistics();

//DEBUG bit
int debug = 0;

int main() {

    FILE *fp;

    int set_associativity;
    int offsetbits;
    int indexbits;
    char *allocation_policy;
    char *write_policy;


    fp = fopen("parameters.txt", "r");
    int i;
    for(i = 0; i < 5; i++){
        if(i == 0){
            fscanf(fp, "%d", set_associativity);
        }else if(i == 1){
            fscanf(fp, "%d", offsetbits);
        }else if(i == 2){
            fscanf(fp, "%d", indexbits);
        }else if(i == 3){
            fscanf(fp, "%s", allocation_policy);
        }else{
            fscanf(fp, "%s", write_policy);
        }
    }

    if(debug){
        printf("set associativity: %d", set_associativity);
        printf("offset bits: %d", offsetbits);
        printf("index bits: %d", indexbits);
        printf("allocation policy: %s", allocation_policy);
        printf("write policy: %s", write_policy);
    }

    fclose(fp);

    statistics *stats;
    stats = Statistics();

    // PrintStatistics(stats);
    // free(stats);

    return 0;    
}

statistics * Statistics(){
    statistics *newstat = (statistics *) malloc(sizeof(statistics));
    if(newstat == NULL){
        return NULL;
    }

    newstat->rhits = 0;
    newstat->whits = 0;
    newstat->rmisses = 0;
    newstat->wmisses = 0;
    newstat->wb = 0;
    newstat->wt = 0;

    return newstat;
}

void PrintStatistics(statistics *stats){
    FILE *out;
    out = fopen("statistics.txt", "w");

    fprintf(out, "rhits: %d\n", stats->rhits);
    fprintf(out, "whits: %d\n", stats->whits);
    fprintf(out, "rmisses: %d\n", stats->rmisses);
    fprintf(out, "wmisses: %d\n", stats->wmisses);

    int total = stats->rhits + stats->whits + stats->rmisses + stats->wmisses;
    double hitrate = (double)(stats->rhits + stats->whits) / total;

    fprintf(out, "hrate: %lf\n", (hitrate * 100));
    fprintf(out, "wb: %d\n", stats->wb);
    fprintf(out, "wt: %d\n", stats->wt);

    fclose(out);
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