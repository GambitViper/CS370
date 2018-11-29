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
int debug = 1;

int main() {

    FILE *params_file;

    int set_associativity;
    int offsetbits;
    int indexbits;
    char allocation_policy[4];
    char write_policy[3];

    params_file = fopen("parameters.txt", "r");

    fscanf(params_file, "%d", &set_associativity);
    fscanf(params_file, "%d", &offsetbits);
    fscanf(params_file, "%d", &indexbits);
    fscanf(params_file, "%3s", allocation_policy);
    fscanf(params_file, "%2s", write_policy);

    if(debug){
        printf("~~~Parameters Debug~~~\n");
        printf("set associativity: %d\n", set_associativity);
        printf("offset bits: %d\n", offsetbits);
        printf("index bits: %d\n", indexbits);
        printf("allocation policy: %s\n", allocation_policy);
        printf("write policy: %s\n", write_policy);
    }

    fclose(params_file);

    statistics *stats;
    stats = Statistics();

    FILE *access_file;

    access_file = fopen("accesses.txt", "r");
    

    return 0;

    
    

    // PrintStatistics(stats);
    // free(stats); 
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

    paramsrintf(out, "rhits: %d\n", stats->rhits);
    paramsrintf(out, "whits: %d\n", stats->whits);
    paramsrintf(out, "rmisses: %d\n", stats->rmisses);
    paramsrintf(out, "wmisses: %d\n", stats->wmisses);

    int total = stats->rhits + stats->whits + stats->rmisses + stats->wmisses;
    double hitrate = (double)(stats->rhits + stats->whits) / total;

    paramsrintf(out, "hrate: %lf\n", (hitrate * 100));
    paramsrintf(out, "wb: %d\n", stats->wb);
    paramsrintf(out, "wt: %d\n", stats->wt);

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