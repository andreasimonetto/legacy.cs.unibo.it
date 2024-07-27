#!/bin/bash

# Build source files that needs macro-expansion to be "readable". These files
# are in the form <file>.unexp.c (they can be executed, but not well-read).

if ! which indent > /dev/null; then
	echo "You need <indent> to run this script."
	exit 1
fi

CUTPOINT="CUT POINT. DON'T REMOVE THIS LINE"
INCLUDE=-I../include
CFLAGS=`cat ../config.mk | sed -ne "s/CFLAGS *= *\(.*\) -c \(.*\)/\1\2/p"`
LIBRARY=`cat ../config.mk | sed -ne "s/LIBRARY *= *\(.*\)/\1/p"`
SRCDIR=`cat ../config.mk | sed -ne "s/SRCDIR *= *\(.*\)/\1/p"`
LIBUMPS_INCLUDE=`cat ../config.mk | sed -ne "s/LIBUMPS_INCLUDE *= *\(.*\)/\1/p"`

for src in *.unexp.c; do
	target=`echo $src | sed -ne "s/\(.*\).unexp.c/\1/p"`
	
	if [ "$1" == "-d" ]; then
		if [ -e "$target.c" ]; then
			echo "Removing $target.c..."
		rm -f $target.c 
		fi
		rm -f $target.exp.c
	else
		echo "Building $target.c..."
		cpp -P -CC -dI $INCLUDE $LIBUMPS_INCLUDE $src -o - | indent -kr -bap -bad -o $target.exp.c
		cutpoint=`cat -n $target.exp.c | grep "$CUTPOINT"|sed -ne "s/^ *\(.*\)\/\* *-- $CUTPOINT.*/\1/p"`
		if [ ! -z $cutpoint ]; then
			lines=`wc -l $target.exp.c | sed -ne "s/^\([0-9]*\) .*/\1/p"` 
			let printlast=$lines-$cutpoint
			tail -n $printlast $target.exp.c > $target.c
			rm $target.exp.c
		else
			echo $src: CUT POINT not found.
		fi
	fi
done
