#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "../../includes/php_combined_lcg.h"

void test(){
	LCG_STATE s;
	uint32_t s1, s2;
	/******************************************************/
  // [*] php_combined_lcg
  // seeding
  s.sec= 1490021419;
  s.usec= 682444;
  if ( s.usec << 11 != 1397645312 )
    printf("shl 1 failed\n");
  s1= 193039403;
  if ( ( s.sec ^ ( s.usec << 11 ) ) != 193039403 )
    printf("s1 failed\n");
  s.pid= 30329;
  // sec2:  1490021419 ;
  s.delta = 688170 - s.usec;
  if ( ( s.usec + s.delta ) << 11 != 1409372160 )
    printf("shl 2 failed\n");
  s2 = 1409361529;
  if (  ( s.pid ^ ( ( s.usec + s.delta ) << 11 ) ) != 1409361529 )
    printf("s2 failed\n");
  // finished seeding
  printf("int: 732611321\n");
  double val = 3.41148740;
  // [*] php_combined_lcg
  printf("second go: 832602756\n");
  double val2 = 3.87710882;
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
	test();
  printf("===========================================\n");
  printf("=====     php_combined_lcg passed     =====\n");
  printf("===========================================\n");
	return 0;
}
