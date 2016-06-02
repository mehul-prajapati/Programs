#!/bin/bash
# This script looks in the current directory
# and then it prints the file which contains
# POSIX string 

. ./file_exist.sh

echo $*

for file in *
do
	if grep -q POSIX $file
	then 
		echo $file 
	fi	
done

#exit 0
