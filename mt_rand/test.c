#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../includes/mt_rand.h"
#include "../includes/php_combined_lcg.h"



int test_full(uint32_t *delt){
  suseconds_t usec_start, usec_end; 
  uint32_t rand;
  LCG_STATE lcg;
  // to save old sec in case delta increases it
  time_t sec1, sec2; 

  double lcg_ret;
  FILE *fp;
  fp = popen(PHP_FILE_MACRO, "r");
  if ( fp == NULL ){
    fprintf(stderr, "Could not popen\nYou probably moved some files arround, just remake\n");
    exit(1);
  }

  fscanf(fp, 
    "sec_start: %ld sec_end: %ld pid: %u usec_start: %ld usec_end: %ld rand: %u", 
    &sec1, &sec2, &lcg.pid, &usec_start, &usec_end, &rand
  );
  fclose(fp);

  if ( sec1 != sec2 ){
    printf("\n\nHit sec overflow, try again\n\n");
    fflush(stdout);
    return test_full(delt);
  }
  lcg.sec = sec1;
  for (lcg.delta=0; lcg.delta<100; lcg.delta++){
    for (lcg.usec=usec_start; lcg.usec < usec_end; lcg.usec++){
      lcg.seeded=0;
      lcg_ret = php_combined_lcg(&lcg);
      if (rand == mt_rand(generate_seed(&lcg, lcg_ret))){
        *delt+=lcg.delta;
        return 0;
      }
    }
  }

  fprintf(
    stderr,
    "\n[!] failed:"
    " sec: %ld  pid: %u usec_start: %ld usec_end: %ld rand: %u\n",
    sec1, lcg.pid, usec_start, usec_end, rand
  );
  return 1;
}

uint32_t slow_mt_rand(uint32_t seed){
  uint32_t ret;
  char cmd[64];
  snprintf(cmd,  sizeof(cmd)-1, "php -r 'mt_srand(%u); echo mt_rand();'", seed);

  FILE *fp;
  fp = popen(cmd, "r");
  if ( fp == NULL ){
    fprintf(stderr, "Could not popen\n");
    exit(1);
  }

  fscanf(fp, "%u", &ret);
  fclose(fp);
  return ret;
}

int main(int argc, char *argv[]){
  int i, max, ret=0;
  uint32_t delt=0;
  uint32_t seed = mt_rand(time(NULL));

  if ( argc != 2 || !(max = atoi(argv[1])))
    max = 100;

  /*
   * verify that php's mt_rand() == our mt_rand when both are given the same
   * seed value
   */
  for ( i=0; i<max/2; i++ ){
    printf("\e[0G[**] checking seed 0x%08x  %.1f%% completed", seed, 100 * (double) ( i+1 )/( max/2 ));
    fflush(stdout);
    if ( slow_mt_rand(seed) != mt_rand(seed) ){
      fprintf(stderr, "\n\nFailed on seed: %u\n", i);
      exit(1);
    }
    seed++;
  }
  printf("\n");

  /*
   * More complicated test to try and discover a mt_rand value given
   * information about the time it was created and the pid 
   */
  for (i=0; i<max; i++){
    ret += test_full(&delt);
    printf("\e[2K\e[0G[**] testing %4d/%d: failed: %3d delt: %.4f ",
      i+1, max, ret, (double) delt/max
    );
    fflush(stdout);
  }
  printf("\n");

  if ( ret ){
    fprintf(stderr, "failed %d tests %.2f%% \n", ret, 100 * (double) ret/max);
    return 1;
  }else{
    printf("average delt1: %.3f\n", (double) delt/max);
    printf("===========================================\n");
    printf("=====        mt_rand passed           =====\n");
    printf("===========================================\n");
  }
	return 0;
}
