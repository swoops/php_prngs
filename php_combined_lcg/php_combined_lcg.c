#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../includes/php_combined_lcg.h"

void lcg_seed(LCG_STATE *s){
  s->s1 = s->sec ^ ( s->usec << 11 );
  s->s2 = s->pid ^ ( (s->usec + s->delta) << 11 );
  s->seeded = 1;
}

#define MODMULT(a, b, c, m, s) q = s/a;s=b*(s-a*q)-c*q;if(s<0)s+=m
double php_combined_lcg(LCG_STATE *s) {
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

	return z * 4.656613e-10;
}

int32_t generate_seed( LCG_STATE *s, double lcd){
  return (((int32_t) (s->sec * s->pid)) ^ ((int32_t) (1000000.0 * lcd)));
}

