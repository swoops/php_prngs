#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "../includes/uniqid.h"
#include "../includes/mt_rand.h"
#include "../includes/php_combined_lcg.h"

int byte_cmpr(char *s1, char *s2, uint32_t size){
  uint32_t i;
  for (i=0; i<size; i++)
    if ( s1[i] != s2[i] )
      return i+1;
  return 0;
}

int test_full(suseconds_t *delt1, suseconds_t *delt2){
  suseconds_t usec_start, usec_end, i; 
  uint32_t rand;
  char ubuff[42];
  char test[42];
  double php_lcg;
  LCG_STATE lcg;

  FILE *fp;
  fp = popen(PHP_FILE_MACRO, "r");
  if ( fp == NULL ){
    fprintf(stderr, "Could not popen\n");
    exit(1);
  }

  fscanf(fp, 
    "sec: %ld pid: %u usec_start: %ld usec_end: %ld rand: %u uniqid: %s \n", 
    &lcg.sec, &lcg.pid, &usec_start, &usec_end, &rand, ubuff
  );
  fclose(fp);

  for (lcg.delta=0; lcg.delta<100; lcg.delta++){
    for (lcg.usec=usec_start; lcg.usec < usec_end; lcg.usec++){
      lcg.seeded=0;

      if ( rand == mt_rand(generate_seed(&lcg, php_combined_lcg(&lcg))) ){
        /* got the mt_rand() so keeps stats on delta */
        php_lcg = php_combined_lcg(&lcg);
        *delt1 += lcg.delta;

        /* second delta, most likly larger */
        for ( i=0; i<100; i++){
          uniqid( test, rand, lcg.sec, lcg.usec+i, php_lcg);
          /* you got it!!! */
          if (!byte_cmpr(test, ubuff, strlen(ubuff))){
            *delt2 += i;
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

int main(int argc, char *argv[]){
  suseconds_t delt1=0, delt2=0; 
  int ret=0;
  int i, max;

  if ( argc != 2 || !(max = atoi(argv[1])))
    max = 100;
    

  for (i=0; i<max; i++){
    ret += test_full(&delt1, &delt2);
    printf("\e[2K\e[0G[**] testing %d/%d: failed: %d delt1: %.4f delt2: %4f", 
      i+1, max, ret, (double) delt1/max, (double) delt2/max
    );
    fflush(stdout);
  }
  printf("\n\n");

  if ( ret ){
    fprintf(stderr, "failed %d tests %.2f%% \n", ret, 100 * (double) ret/max);
    return 1;
  }else{
    printf("average delt1: %.3f\naverage delt2: %.3f\n", (double) delt1/max, (double) delt2/max);
    printf("===========================================\n");
    printf("=====         uniqid passed           =====\n");
    printf("===========================================\n");
  }
	return 0;
}
