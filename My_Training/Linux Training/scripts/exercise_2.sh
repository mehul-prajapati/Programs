#!/bin/bash
if [ "$(pidof foo)" ] 
then
	echo process was found
	kill `pidof foo`
else
	echo process not found
fi

