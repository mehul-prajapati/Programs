#!/bin/bash
i=1
while [ $i -le 10 ]
do
	echo count $i
	mkdir Stage_$i
	i=$[$i+1]
done
