<?php
	function write_pid($pid){

		$fp = fopen("/tmp/pid_me", "w");

		if ( ! $fp ) {
			return False;
		}

		$str = "pid: " . $pid . "\n";
		$ret = fwrite($fp, $str );
		fclose($fp);
		return True;
	}


	$pid = getmypid();
	if ( write_pid($pid) )
		echo "File made sleeping\n";
	else
		die("could not make file?\n");

	if(isset($_GET['nosleep']) && !empty($_GET['nosleep']))
		echo "skipped sleep";
	else
		sleep(3);

	$before = time();
	$rand = mt_rand();
	$uni = uniqid($rand, TRUE);
	$hash = sha1($uni);
	$after = time();


  printf("mt_rand: 0x%x %10d\n", $rand, $rand);
  printf("pid:     0x%x %10d\n", $pid, $pid);
  echo "uniqid:  " . $uni . "\n";


  $obj = 'a:4:{s:13:":old:username";s:4:"asdf";s:12:":old:message";s:38:"That is the wrong username or password";s:13:":new:username";s:4:"asdf";s:12:":new:message";s:38:"That is the wrong username or password";}';

  $obj_out = "'" . $obj . md5($obj . $hash) . "'";

  echo "exploit -o $obj_out -p $pid -P $pid -t $before -T $after\n";

