#!/bin/bash
if [ $# -ne 1 ]; then
	echo "Please pass argument"
	exit 0
fi

num=$1
sum=0
while [ $num -gt 0 ]
do
	sum=`expr $sum + $num % 10`
	num=`expr $num / 10`
done

echo "sum = $sum"

