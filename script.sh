#!/bin/bash

#read -p "Please enter make or distclean or cp: " BEHAVE
#echo "Your Enter Is: $BEHAVE"
BEHAVE=$1

var1="make"
var2="distclean"
var3="copy"

if [ $BEHAVE == $var1 ]
then
	make
elif [ $BEHAVE == $var2 ]
then
	make distclean
elif [ $BEHACE == $var3 ]
then
	cp drew.bin /home/book/tftpboot/ -f
else
	echo "Error ,please enter again!"
fi







