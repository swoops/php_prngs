#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../includes/mt_rand.h"


uint32_t mt_rand(uint32_t seed){
  uint32_t p1 = ( 1812433253U * ( seed ^ (seed >> 30) ) + 1 ) & 0xffffffffU;
  uint32_t i;
  uint32_t holder = p1;

  for(i=2; i < 398; ++i ) {
    holder = ( 1812433253U * ( holder ^ (holder >> 30) ) + i ) & 0xffffffffU;
  }
  seed = twist_php(holder, seed, p1);
  seed ^= (seed >> 11);
  seed ^= (seed <<  7) & 0x9d2c5680U;
  seed ^= (seed << 15) & 0xefc60000U;
  return (seed ^ (seed >> 18)) >> 1;
}
