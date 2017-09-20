#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "../includes/php_combined_lcg.h"

int test_full(){
  suseconds_t usec_start, usec_end;
  char ubuff1[42];
  char ubuff2[42];
  char test[12];
  char *unid, *unid2;
  LCG_STATE lcg;

  FILE *fp;
  fp = popen(PHP_FILE_MACRO, "r");
  if ( fp == NULL ){
    fprintf(stderr, "Could not popen\n");
    exit(1);
  }

  fscanf(fp, 
    "pid: %u sec: %ld before: %ld after: %ld uniqid: asdf%s uniqid2: asdf%s", 
    &lcg.pid, &lcg.sec, &usec_start, &usec_end, ubuff1, ubuff2
  );
  fclose(fp);

  unid  = ubuff1+strlen(ubuff1)-10;
  unid2 = ubuff2+strlen(ubuff2)-10;
  /* printf("trying to find uniqid: %s\n", unid); */

  for (lcg.delta=0; lcg.delta<100;lcg.delta++){
    for (lcg.usec=usec_start; lcg.usec < usec_end; lcg.usec++){
      lcg.seeded=0;
      snprintf(test, sizeof(test), "%.8F", 10*php_combined_lcg(&lcg));
      if (strcmp(test, unid) == 0){
        snprintf(test, sizeof(test), "%.8F", 10*php_combined_lcg(&lcg));
        if (strcmp(test, unid2) == 0){
          return 0;
        }
      }
    }
  }
  return 1;
}

void test(){
	LCG_STATE s;
	uint32_t s1, s2;
	/******************************************************/
  // [*] php_combined_lcg
  // seeding
  s.sec= 1488813417;
  s.usec= 656518;
  if ( s.usec << 11 != 1344548864 )
    printf("shl 1 failed\n");
  s1= 144264553;
  if ( ( s.sec ^ ( s.usec << 11 ) ) != 144264553 )
    printf("s1 failed\n");
  s.pid= 4272;
  // sec2:  1488813417 ;
  s.delta = 662414 - s.usec;
  if ( ( s.usec + s.delta ) << 11 != 1356623872 )
    printf("shl 2 failed\n");
  s2 = 1356619952;
  if (  ( s.pid ^ ( ( s.usec + s.delta ) << 11 ) ) != 1356619952 )
    printf("s2 failed\n");

  // finished seeding
  printf("int: 1942657870\n");
  double val = 9.04620589;
  // [*] php_combined_lcg
  printf("int: 879787869\n");
  double val2 = 4.09683163;
  // 4.09683163
	/*******************************************************/

	lcg_seed(&s);
  if ( s1 != s.s1 ){
    printf("failed s1\n");
    exit(1);
  }else if ( s2 != s.s2 ){
    printf("failed s2\n");
    exit(1);
  }

  printf("%.8F\n%.8F\n", php_combined_lcg(&s)*10, val);
  printf("%.8F\n%.8F\n", php_combined_lcg(&s)*10, val2);
}


int main(int argc, char *argv[]){
  int i,ret = 0, max=300;
  if ( argc == 2 )
    if ( (max = atoi(argv[1])) <= 0 )
      max = 300;
    
	test();
  printf("max: %d\n", max);
  for (i=0; i<max; i++){
    if (test_full()){
      ret++;
    }
    printf("\e[0G[**] testing %d %d/%d succeeded", i+1, i-ret+1, max);
    fflush(stdout);
  }
  printf("\n\n");

  if ( ret ){
    printf("[!!] failed %d/%d tests %.5F%%\n", ret, max, ( (double)ret/max )*100 );
  }else{
    printf("===========================================\n");
    printf("=====     php_combined_lcg passed     =====\n");
    printf("===========================================\n");
  }
	return 0;
}
