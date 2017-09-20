#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>

/*
 * must pass it a buffer larger then something maybe 32? yeah that sounds good
*/
void uniqid(char *buff, uint32_t rand, time_t sec, suseconds_t usec, double lcg){
  snprintf(buff, 41,  "%u%08x%05x%.8F", 
    rand, (unsigned int) sec, (unsigned int) usec, lcg * 10 
  );
}
