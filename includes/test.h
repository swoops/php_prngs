
#define GITTER_MAX 100
void test_full(){
  uint32_t i,lookn;
  suseconds_t usec_start, usec_end;
  LCG_STATE s;
  s.sec           = 1488684570;
  s.pid           = 8121;
  usec_start      = 327208;
  usec_end        = 327218;
  lookn           = 1915931527;
  char lookn2[]   = "191593152758bb861a4fe3f4.49514614";

  for (s.delta=0; s.delta < GITTER_MAX; s.delta++){
    for (s.usec=usec_start; s.usec < usec_end; s.usec++){
      if ( check(&s, (uint32_t) lookn) == 0 ) goto yay;
    }
  }
  printf("YOU failed mtrand()!!!\n\n");
  exit(1);

  yay:
  printf("Found mt_rand()!\n");

  double lcg = php_combined_lcg(&s);
  char buff[42];
  printf("%8F\n", lcg*10);
  printf("%s\n", lookn2);

  for (i=0; i<20; i++){
    uniqid(buff, lookn, s.sec, i+s.usec, lcg);
    if (strcmp(buff, lookn2) == 0 ){
      printf("You got it!!!\n");  
	  return;
    }
    
  }
    printf("YOU failed uniqid()!!!\n\n");
    exit(1);
}
