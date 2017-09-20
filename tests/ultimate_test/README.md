# Ultimate Test

This directory is left only for debugging purposes. It is junk I wrote to try
and figure out a very rare discrepancy in mt_rand and uniqid tests. I gave up,
so I left this here in case I decide to try it again one day.

The ultimatetest binary is used to run A LOT of tests. To get the full
potential you need to debug the PHP process so the test_making directory
contains some scripts to use. The gdb script included likely needs changing
because offsets will be different for different compilations of PHP.

However you can run my tests, they are all successful on my system, to find
where your system might be discrepant. Just grep out "ultimate_test" from the
"./test_making/log_file" and run them. 
