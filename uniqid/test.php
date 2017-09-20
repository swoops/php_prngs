<?php
	$before = microtime();
	$r = mt_rand();
	$pid = getmypid();
	$after = microtime();


	$a = explode(" ", $before);
	$sec1 = $a[1];
	$usec1 = intval(explode(".", $a[0])[1]) / 100;

	$a = explode(" ", $after);
	$sec2 = $a[1];
	$usec2 = intval(explode(".", $a[0])[1]) / 100;
	$lookn2 = uniqid($r, true);

	if ( $sec1 != $sec2 ){
		echo "seconds don't match!!!\n";
    exit(2);
	}
  echo "sec: $sec1 pid: $pid usec_start: $usec1 usec_end: $usec2 rand: $r uniqid: $lookn2 \n";

