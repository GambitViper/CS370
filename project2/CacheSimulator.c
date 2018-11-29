#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
    int *blocktoreplace;
};
typedef struct cache_set cache_set;
void PrintStatistics(statistics *);
statistics * Statistics();
cache_set * CacheSet(int);
void SimulateCache(cache_set **, statistics *, int, unsigned int, unsigned int, unsigned int, int, int, int, int);

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

    unsigned int mem_addr;
    char read_write[2];

    statistics *stats;
    stats = Statistics();

    int block_size = pow(2, offsetbits);
    int set_count = pow(2, indexbits);
    int is_wa = strcmp(allocation_policy, "wa") ? 0 : 1;
    int is_wt = strcmp(write_policy, "wt") ? 0 : 1;

    cache_set *cache[set_count];
    int i;
    for(i = 0; i < set_count; i++){
      cache_set *set;
      set = CacheSet(set_associativity);
      cache[i] = set;
    }

    if(debug){
      printf("~~~Cache Statistics~~~\n");
      printf("block size: %d\n", block_size);
      printf("set_count: %d\n", set_count);
      printf("isWriteAllocate: %s\n", (is_wa) ? "true" : "false");
      printf("isWriteThrough: %s\n", (is_wt) ? "true" : "false");
      printf("~~~Tracing accesses.txt file~~~\n");
    }

    FILE *access_file;

    access_file = fopen("accesses.txt", "r");
    int timetrack = 0;

    while(fscanf(access_file, "%s %x", read_write, &mem_addr) != EOF){
      unsigned int index, offset, tag;
      offset = GetHexValue(mem_addr, offsetbits - 1, 0);
      index = GetHexValue(mem_addr, (indexbits + offsetbits) - 1, offsetbits);
      tag = GetHexValue(mem_addr, 31, indexbits + offsetbits);
      int is_read = strcmp(read_write, "r") ? 0 : 1;

      if(debug){
        printf("read/write: %s\n", read_write);
        printf("memory addr: %x\n", mem_addr);
        printf("\toffset--> %d\n", offset);
        printf("\tindex--> %d\n", index);
        printf("\ttag--> %x\n", tag);
      }

      SimulateCache(cache, stats, set_associativity, offset, index, tag, is_wa, is_wt, timetrack, is_read);

      timetrack++;
    }

    return 0;




    // PrintStatistics(stats);
    // free(stats);
}

void SimulateCache(cache_set **cache, statistics *stats, int ways, unsigned int offset, unsigned int index, unsigned int tag, int is_wa, int is_wt, int timetrack, int is_read){
  int lastway = 0;
  int lasttime = timetrack;
  int way;

  for(way = 0; way < ways; way++){
    curr_set = cache[way];
    if(curr_set->valid[way] == 1 && curr_set->tag[way] == tag){
      //hit
      if(is_read){
        stats->rhits++;
      }else{
        stats->whits++;
        if(is_wa){

        }
      }
    }else {
      //non-hit but non-exhausted
      if(lasttime > curr_set->blocktoreplace[way]){
        lasttime = curr_set->blocktoreplace[way];
        lastway = w;
      }
    }
  }

  if(is_read){
    stats->rmisses++;
  }else {
    stats->wmisses++;
  }


}

cache_set * CacheSet(assoc){
  cache_set *newset = (cache_set *) malloc(sizeof(cache_set));
  if(newset == NULL){
    return NULL;
  }

  newset->valid = (int *) malloc(sizeof(int) * assoc);
  newset->dirty = (int *) malloc(sizeof(int) * assoc);
  newset->tag = (unsigned int *) malloc(sizeof(unsigned int) * assoc);
  newset->blocktoreplace = (int *) malloc(sizeof(int) * assoc);

  int i;
  for(i = 0; i < assoc; i++){
    newset->valid[i] = 0;
    newset->dirty[i] = 0;
    newset->tag[i] = 0;
    newset->blocktoreplace[i] = 0;
  }



  return newset;
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
