#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "../../includes/php_combined_lcg.h"

#define MODMULT(a, b, c, m, s) q = s/a;s=b*(s-a*q)-c*q;if(s<0)s+=m
double debug_php_combined_lcg(LCG_STATE *s) {
	int32_t q;
	int32_t z;

	if (s->seeded!=1) 
    lcg_seed(s);

	MODMULT(53668, 40014, 12211, 2147483563L, s->s1);
	MODMULT(52774, 40692, 3791, 2147483399L, s->s2);

	z = s->s1 - s->s2;
	if (z < 1) {
		z += 2147483562;
	}
  printf("DEBUG int %u\n", z);

	return z * 4.656613e-10;
}

int test_spec(uint32_t sec, uint32_t usec, uint32_t pid, uint32_t delta, uint32_t s1, uint32_t s2){
  LCG_STATE lcg;
  lcg.pid     = pid;
  lcg.sec     = sec;
  lcg.usec    = usec;
  lcg.delta   = delta;
  lcg.seeded  = 0;

  printf("==== before === \n");
  printf("\tlcg.pid:    %u\n", lcg.pid);
  printf("\tlcg.sec:    %u\n", lcg.sec);
  printf("\tlcg.usec:   %u\n", lcg.usec);
  printf("\tlcg.delta:  %u\n", lcg.delta);
  printf("\tlcg.s1:     %u\n", s1);
  printf("\tlcg.s2:     %u\n", s2);

  printf("+++ first LCG +++\n");
  printf("lcg: %.8F\n", php_combined_lcg(&lcg)*10);
  printf("s1: %u\ns2:%u\n", lcg.s1, lcg.s2);
  printf("+++ second LCG +++\n");
  printf("lcg: %.8F\n", php_combined_lcg(&lcg)*10);
  printf("s1: %u\ns2:%u\n", lcg.s1, lcg.s2);


  return 1;
}
int test(char *uniqid_str){
  uint32_t usec_start, usec_end;
  char ubuff1[42];
  char ubuff2[42];
  char test[12];
  char *unid, *unid2;
  LCG_STATE lcg;

  sscanf(uniqid_str, 
    "pid: %u sec: %u before: %u after: %u uniqid: asdf%s uniqid2: asdf%s", 
    &lcg.pid, &lcg.sec, &usec_start, &usec_end, ubuff1, ubuff2
  );

  if ( !lcg.pid || !lcg.sec || !usec_start || !usec_end ){
    fprintf(stderr, "Looks like your input was stupid!\n");
    exit(1);
  }

  printf("\tlcg.pid: %u\n", lcg.pid);
  printf("\tlcg.sec: %u\n", lcg.sec);
  printf("\tusec_start: %u\n", usec_start);
  printf("\tusec_end: %u\n", usec_end);
  printf("\tubuff1: %s\n", ubuff1);
  printf("\tubuff2: %s\n", ubuff2);

  unid  = ubuff1+strlen(ubuff1)-10;
  unid2 = ubuff2+strlen(ubuff2)-10;
  printf("trying to find uniqid1: %s uniqid2: %s\n", unid, unid2);

  for (lcg.delta=0; lcg.delta<1000;lcg.delta++){
    for (lcg.usec=usec_start; lcg.usec < usec_end; lcg.usec++){
      lcg.seeded=0;
      snprintf(test, sizeof(test), "%.8F", 10*php_combined_lcg(&lcg));
      if (strcmp(test, unid) == 0){
        printf("found 1\n");
        snprintf(test, sizeof(test), "%.8F", 10*php_combined_lcg(&lcg));
        printf("\tlcg.s1:     %u\n", lcg.s1);
        printf("\tlcg.s2:     %u\n", lcg.s2);
        printf("\tlcg.pid:    %u\n", lcg.pid);
        printf("\tlcg.usec:   %u\n", lcg.usec);
        printf("\tlcg.delta:  %u\n", lcg.delta);
        printf("\tlcg.seeded: %u\n", lcg.seeded);
        if (strcmp(test, unid2) == 0){
          printf("Success!\n");
          return 0;
        }
      }
    }
  }
  printf("FAILED!!!\n");
  return 1;
}

int main(int argc, char *argv[]){
  if ( argc != 2 ){
    if ( argc == 7 ){
      test_spec(
        atoi(argv[1]), // sec
        atoi(argv[2]), // usec
        atoi(argv[3]), // pid
        atoi(argv[4]), // delta
        atoi(argv[5]), // s1
        atoi(argv[6])  // s2
      );
    }
    fprintf(stderr, "You must have exactly one param, output of \"php ../test.php\"\n");
    exit(1);
  }
  return test(argv[1]);
}
