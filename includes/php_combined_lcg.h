typedef struct{ 
  time_t sec; 
  suseconds_t usec, delta;
  uint32_t pid;
  int32_t s1, s2;
  uint32_t seeded;
}LCG_STATE;

/* seeds lcg, hit this if LCG_STATE.seed == 0 */
void lcg_seed(LCG_STATE *s);

/* the PRG */
double php_combined_lcg(LCG_STATE *s);

/* used to generate seed for mtrand  maybe should go somewhere else */
int32_t generate_seed( LCG_STATE *s, double lcd);

