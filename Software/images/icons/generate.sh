#!/bin/bash

keypad-sharp-svgrepo-com-pclike.svg
keypad-sharp-svgrepo-com-phonelike.svg

HEIGHTS="40 50 60 70 80 90 100"

for SRC in *.svg; do
	for HEIGHT in $HEIGHTS; do
		DST="$( basename $SRC .svg)-height=$HEIGHT.png"
		[ -f $DST ] && continue
		echo "Generating $DST ..."
		convert \
			-geometry x$HEIGHT \
			+antialias \
			-background transparent \
			$SRC $DST
	done
done
