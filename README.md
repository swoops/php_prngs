# php_prngs
A library of PHP PRNG's for breaking things

# Building
Just run make. All the .o files will be in the "libs" directory. 

# Tests
Making PRNG's is hard, so I tried to write some tests. Run "Make test" to run
some tests:

```
> make test
./tests//php_combined_lcg.test
int: 1942657870
int: 879787869
9.04620589
9.04620589
4.09683163
4.09683163
max: 300
[**] testing 300 300/300 succeeded

===========================================
=====     php_combined_lcg passed     =====
===========================================
./tests//mt_rand.test
[**] checking seed 0x69466d47  100.0% completed
[**] testing  100/100: failed:   0 delt: 0.1000 
average delt1: 0.100
===========================================
=====        mt_rand passed           =====
===========================================
./tests//uniqid.test
[**] testing 100/100: failed: 0 delt1: 0.1800 delt2: 17.560000

average delt1: 0.180
average delt2: 17.560
===========================================
=====         uniqid passed           =====
===========================================
```

There is a small chance a test will fail, which causes "make test" to fail.
