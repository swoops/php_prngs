<?php
function log_out($msg){
    $fp = fopen("/tmp/flog", "a");
    fwrite($fp, "failed: $msg");
    fclose($fp);
}
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
  
  $str =  "sec_start: $sec1 sec_end: $sec2 pid: $pid usec_start: $usec1 usec_end: $usec2 rand: $r \n";
  if ( $sec1 != $sec2 ){
  	log_out($str);
  }

  echo $str;

