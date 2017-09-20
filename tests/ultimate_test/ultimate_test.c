#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


#include "../includes/uniqid.h"
#include "../includes/mt_rand.h"
#include "../includes/php_combined_lcg.h"

#define SUCCESS "\e[92mPASSED \e[00m"
#define FAILED "\e[91mFAILED \e[00m"
int RET;

int seed_test( LCG_STATE *s, int32_t left, int32_t right, int32_t seed){
  int ret = 0;
  int32_t my_left = (int32_t) (s->sec * s->pid);
  int32_t my_right = (int32_t) (1000000.0 * php_combined_lcg(s));

  if ( my_right == right ){
    printf(SUCCESS " right of generate seed matches\n");
  } else {
    printf(FAILED " right of genreate_seed does not match\n");
      RET+=1;
    ret++;
  }

  if ( my_left == left ){
    printf(SUCCESS " left of generate seed matches\n");
  } else {
    printf(FAILED " left of genreate_seed does not match\n");
    printf("  my_left vs real: %d  %d\n", my_left, left);
    printf("  my_right vs real: %d  %d\n", my_right, right);
    ret++;
      RET+=1;
  }

  if ( ( left ^ right ) == seed ){
    printf(SUCCESS " seed of generate seed matches\n");
  } else {
    printf(FAILED " seed of genreate_seed does not match\n");
    ret++;
      RET+=1;
  }

  return ret;
}


void dump_state(LCG_STATE *state){
  printf(
    "state:\n"
    "  seeded:   %u\n"
    "  s1:       0x%08x\n"
    "  s2:       0x%08x\n"
    "  sec:      0x%08lx\n"
    "  usec:     0x%08lx\n"
    "  delta:    0x%08lx\n"
    "  pid:      %d\n",
    state->seeded, state->s1, state->s2,
    state->sec, state->usec, state->delta,
    state->pid
  );
}


int ultimate_test(char *uni,suseconds_t delta){
  char buff[42];
  uint32_t rand, mt_seed, len, ret=0;
  suseconds_t usec_max, usec_min, delta_max=5, delta_min=0;
  double lcg;
  LCG_STATE state;

  strncpy(buff, uni, sizeof(buff));
  len = strlen(buff) - 10;

  lcg = atof(buff+len);
  buff[len] = 0x00;

  len-=5;
  sscanf(buff+len, "%lx", &usec_max);
  buff[len] = 0x00;

  len-=8;
  sscanf(buff+len, "%lx", &state.sec);
  buff[len] = 0x00;

  rand = atoi(buff);

  // if we know the delta we cheat...
  // in the real world delta is on average less then 1 microsecond
  // with gdb attached it is 6000+ microseconds...
  if ( delta ){
    delta_max=delta+1;
    delta_min=delta;

    usec_min = 0;
  }else{
    usec_min = usec_max - 0x100;
  }

  /* uint64_t count = 0; */
  // checking for mt_rand and second php_combined_lcg
  for (state.usec=usec_max; state.usec!=usec_min; state.usec--){
    for (state.pid=1; state.pid<=30000; state.pid++){
      for (state.delta=delta_min; state.delta<delta_max; state.delta++){
        state.seeded = 0;
        mt_seed = generate_seed(&state, php_combined_lcg(&state));

        /* if ( count % 65536 == 0 ){ */
        /*   printf( */
        /*     "\e[G  sec: 0x%08lx usec:  0x%08lx delta:  0x%02lx pid: 0x%08x count: %ld ", */
        /*     state.sec, state.usec, state.delta, state.pid, count */
        /*   ); */
        /*   fflush(stdout); */
        /* } */
        /* count++; */

        // mt_rand is much slower so only do it if php_combined_lcg checks out!
        // php_combined_lcg will hit many times
        if ( fabs( php_combined_lcg(&state)*10 - lcg ) < .000001 ){
          if ( mt_rand(mt_seed) == rand ){
            printf("\nGOT it!! uniqid_delta: 0x%02lx\n", usec_max - state.usec);
            dump_state(&state);
            ret = 1;
            goto end_loops;
          }
        }
      } // delta loop
    } // pid loop
  } // usec loop

  end_loops:

  return ret;
}

int find_s1_s2(int32_t s1, int32_t s2, suseconds_t usec,  LCG_STATE *state){
  int ret=0;
  printf("== lets see if we can find it ==\n");
  printf("check for bad delta\n");

  /* rest of lcg should already be read */
  state->usec = usec;
  for (state->delta=0; state->delta<999; state->delta++){
    state->seeded = 0;
    lcg_seed(state);
    if ( state->s1 == s1 && state->s2 == s2 ){
      printf("Found it!, delta should be: %ld\n", state->delta);
      ret = 1;
    }
  } 
  return ret;
}
void help_menu(char *prog){
  printf(
    "%s flags:\n"
    "  -q s1\n"
    "  -s s2\n"
    "  -l lcg # first ret from php_combined_lcg\n"
    "  -f uniqid\n"
    "  -r rand\n"
    "  -p pid\n"
    "  -t sec\n"
    "  -u usec1\n"
    "  -v usec2\n"
    "  -w usec3\n"
    "  -g delta\n"
    "  -a seed\n"
    "  -R right of generate_seed\n"
    "  -L left of generate seed\n"
    "  -S silent be quieter\n"
    "  -F No tests that take forever\n",
    prog
  );
  exit(1);
}
int main(int argc, char *argv[]){
  /* true variables from gdb */
  uint32_t test_seed;
  int32_t s1=0, s2=0;
  uint32_t pid=0, rand=0, seed=0, left=0, right=0;
  suseconds_t usec1=0, usec2=0, usec3=0, delta=0;
  time_t sec;
  double lcg=0;
  char *uni = NULL;
  int flags = 0;
  #define SILENT 1
  #define NO_LONG 2

  /* for the testing */
  LCG_STATE state;
  double lcg_ret;
  uint32_t test_rand;
  char buff[42];

  RET = 0;
  int c;
  if ( argc == 1 ) help_menu(argv[0]);
  while ((c = getopt(argc, argv, "hSq:s:r:p:t:u:v:w:g:l:f:a:R:L:F")) != -1){
    switch (c) {
      case 'h':
        help_menu(argv[0]);
        break;
      case 'q':
        s1 = atoi( optarg );
        break;
      case 'a':
        seed = atoi( optarg );
        break;
      case 'R':
        right = atoi( optarg );
        break;
      case 'L':
        left = atoi( optarg );
        break;
      case 's':
        s2 = atoi( optarg );
        break;
      case 'l':
        lcg = atof( optarg );
        break;
      case 'r':
        rand = atoi( optarg );
        break;
      case 'p':
        pid = atoi( optarg );
        break;
      case 't':
        sec = atoi( optarg );
        break;
      case 'u':
        usec1 = atoi( optarg );
        break;
      case 'v':
        usec2 = atoi( optarg );
        break;
      case 'w':
        usec3 = atoi( optarg );
        break;
      case 'g':
        delta = atoi( optarg );
        break;
      case 'f':
        uni = optarg;
        break;
      case 'S':
        flags |= SILENT;
        break;
      case 'F':
        flags |= NO_LONG;
        break;
      case '?':
      default:
        printf("something swoopsed\n");
        help_menu(argv[0]);
    }
  } // end switch

  if ( usec1 && !usec2 ){
    if ( delta ){
      if ( delta == usec2 - usec1 ){
        printf("delta checks out\n");
      }else{
        printf("delta is not usec2-usec1\n");
      }
    }else{
      delta = usec2-usec1;
    }
  }

  if ( ( flags&SILENT ) == 0 ){
    printf(
      "I got this: \n"
      "  s1:           0x%08x\n"
      "  s2:           0x%08x\n"
      "  lcg:          %.8F\n"
      "  uniqid:       %s\n"
      "  rand:         0x%08x\n"
      "  pid:          0x%08x\n"
      "  sec:          0x%08lx\n"
      "  usec1:        0x%08lx\n"
      "  usec2:        0x%08lx\n"
      "  usec3:        0x%08lx\n"
      "  delta:       0x%08lx\n"
      "  seed:         0x%08x\n"
      "  left:         0x%08x\n"
      "  right:        0x%08x\n",
      s1, s2, lcg, uni ? uni : "Not given", rand, pid, sec, 
      usec1, usec2, usec3,  delta, seed, left, right
    );
  }

  // hit an increase in second, currently the code does not know how to deal
  // with that
  if ( usec1 > usec2 ){
    fprintf(stderr, 
      "[!!] usec1 > usec2 I can't deal with this in my life right now!\n"
      "  -u %ld -v %ld -w %ld \n",
      usec1, usec2, usec3
    );
    return 0;
  }

  int fail = 0;
  if ( pid && sec && usec1 && s1 && s2){
    state.pid = pid;
    state.sec = sec;
    state.usec = usec1;
    state.delta = delta;
    state.seeded = 0;

    lcg_seed(&state);

    if ( state.s1 == s1 ) {
      printf(SUCCESS "s1 checks out from pid,sec,usec,delta\n");
    } else {
      printf(FAILED " s1 not from pid,sec,usec,delta\n");
      fail = 1;
      RET+=1;
    }

    if ( state.s2 == s2 ) {
      printf(SUCCESS "s2 checks out from pid,sec,usec,delta\n");
    } else {
      printf(FAILED " s2 not from pid,sec,usec,delta\n");
      RET+=1;
      fail = 1;
    }

    if (fail){
      state.sec = sec;
      state.pid = pid;
      if ( find_s1_s2(s1, s2, usec1, &state) ){
        printf("=== figured it out!!! ==== \n");
        printf("Delta should be: %ld\n", state.delta);
        printf("replacing delta\n");
        delta = state.delta;
      }else{
        printf(FAILED "could not figure out delta\n");
        RET+=1;
      }
    }
  }
  fail = 0;

  if ( s1 && s2 ){
    state.seeded = 1;
    state.s1 = s1;
    state.s2 = s2;
    lcg_ret = php_combined_lcg(&state)*10;
    if ( fabs( lcg - lcg_ret ) < 0.00000001 ){
      printf(SUCCESS "s1 & s2 do make lcg!\n");
    }else{
      printf(FAILED "php_lcg_rand had a failure\n");
      RET+=1;
      printf("  true lcg (%.8F) - mine (%.8F): %.8F \n", lcg, lcg_ret, fabs( lcg - lcg_ret ));
    }
  }

  // mt_rand testiing
  if ( seed && left && right && s1 && s2){
    state.s1 = s1;
    state.s2 = s2;
    state.seeded = 1;
    seed_test(&state, left, right, seed);
  }

  if ( seed && s1 && s2 ){
    state.s1 = s1;
    state.s2 = s2;
    state.seeded = 1;
    test_seed = generate_seed(&state, php_combined_lcg(&state));

    if ( seed == test_seed ){
      printf(SUCCESS "s1,s1->seed\n");
    }else{
      printf(FAILED " s1,s1->seed!!!\n");
      RET+=1;
    }
  }
  if ( rand && s1 && s2 ){
    state.s1 = s1;
    state.s2 = s2;
    state.seeded = 1;

    state.s1 = s1;
    state.s2 = s2;
    state.seeded = 1;
    test_seed = generate_seed(&state, php_combined_lcg(&state));
    test_rand = mt_rand(test_seed);
    if ( test_rand == rand ){
      printf(SUCCESS "s1, s2-> mt_rand properly\n");
    }else{
      printf(FAILED " my mt_rand: %u != %u\n", test_rand, rand);
      RET+=1;
      printf("  lcg for seed: %d\n", test_seed);
      printf("     vs       : %d\n", seed);
    }
  }

  if ( rand && seed ){
    test_rand = mt_rand(seed);
    if ( test_rand == rand ){
      printf(SUCCESS "seed -> mt_rand properly\n");
    }else{
      printf(FAILED " my mt_rand: %u != %u\n", test_rand, rand);
      RET+=1;
    }
  }


  // uniqid
  if ( uni && rand && sec && usec3){
    state.s1 = s1;
    state.s2 = s2;
    php_combined_lcg(&state);

    uniqid(buff, rand, usec2 > usec3 ? sec+1 : sec, usec3, php_combined_lcg(&state));
    if ( strcmp(buff, uni) == 0 ){
      printf(SUCCESS "rand, sec, usec3 -> uniqid\n");
    }else{
      printf( 
        FAILED "rand, sec, usec3 -!> uniqid\n"
        "\tmine: %s\n"
        "\ttrue: %s\n",
        buff, uni
      );
      RET+=1;
    }
  }

  // all together now
  if ( uni && sec && usec1 && delta && pid && usec3) {
    state.sec     =  sec;
    state.pid     =  pid;
    state.usec    =  usec1;
    state.delta   =  delta;
    state.seeded  =  0;

    test_rand = mt_rand(generate_seed(&state, php_combined_lcg(&state))); 
    if ( usec2 > usec3 ){
      state.sec = sec+1;
    }
    uniqid( buff, rand, state.sec, usec3, php_combined_lcg(&state));

    if ( strcmp(buff, uni) == 0 ){
      printf(SUCCESS "got uniqid from lcg state\n");
    }else{
      printf( 
        FAILED "did not get uniqid from lcg state\n\n"
        "  mine: %s\n"
        "  true: %s\n",
        buff, uni
      );
      RET+=1;
    }
  }

  // this is the brute force test so it takes a while
  // -F flag means don't do it
  if ( ( flags & NO_LONG )  == 0){
    if ( uni ){
      // if we got this value from gdb delta will be MUCH bigger so we cheat then
      if ( ultimate_test(uni, delta) ){
        printf(SUCCESS "YOU GOT IT!!! Ultimate SUCCESS!!!\n");
      }else{
        printf(FAILED "You should probably give up...\n");
        return 1;
      }
    }
  }
  return 0;
}
