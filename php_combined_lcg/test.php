<?php
  $before  =  microtime();
  $pid     =  getmypid();

  $uniqid  =  uniqid("asdf", TRUE);

  $after   =  microtime();


	$a = explode(" ", $before);
	$sec1 = $a[1];
	$usec1 = intval(explode(".", $a[0])[1]) / 100;

	$a = explode(" ", $after);
	$sec2 = $a[1];
	$usec2 = intval(explode(".", $a[0])[1]) / 100;

  echo "pid: $pid sec: $sec1 before: $usec1 after: $usec2 uniqid: $uniqid uniqid2: ". uniqid("asdf", TRUE). "\n";
