#!/bin/bash
if [ -d ./foo ];then
	cd foo
else
	mkdir foo
	cd foo
fi

