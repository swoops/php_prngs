#!/bin/bash

f="/tmp/pid_me" 
if [[ -e $f ]]; then
	echo $f exists, removing it
	rm $f
fi

echo "restarting server"
systemctl restart apache2.service


echo "sending request"
( curl -s "http://127.0.0.1/test.php" >/dev/null&) 


while [[ ! -e $f ]]; do
	# sleep half a second.. stupid I know
	python -c "import time; time.sleep(0.5)" 
done

echo starting debugger

# debug it
gdb -quiet -x ./gdb_scriptrc -p $(cat $f |cut -d' ' -f2)

