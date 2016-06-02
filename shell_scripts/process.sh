#!/bin/bash

while true
do
if (pgrep firefox)
then
	echo "Process found :) "
else
	sleep 2
	firefox
	echo "Crap"
fi
done
