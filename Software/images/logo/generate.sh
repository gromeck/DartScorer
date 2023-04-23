#!/bin/bash

BASENAME="DartScorer-Logo"
HEIGHTS="50 100 200 400 800 1600"

for HEIGHT in $HEIGHTS; do
	for COLOR in white black; do
		SRC="${BASENAME}-${COLOR}.svg"
		DST="${BASENAME}-${COLOR}-height=${HEIGHT}px.png"
		[ -f $DST ] && continue
		echo "Generating $DST ..."
		convert \
			-geometry x$HEIGHT \
			+antialias \
			-background transparent \
			$SRC $DST
	done
done
