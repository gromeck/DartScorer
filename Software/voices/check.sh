#!/bin/bash
#
#	check a voice for completeness
#

WAVS="
out
bull
bullseye
double
tripple
no-score
game-on
won-leg
won-set
won-game
busted
"
N=0
while [ $N -le 180 ]; do
	WAVS="$WAVS $N"
	N=$(( $N + 1 ))
done

MAX_FILESIZE=0

for DIR in $( ls ); do
	if [ -d $DIR ] && [ $DIR/game-on.wav ]; then
		echo -n "Checking voice $DIR for completeness ... "
		MISSING=0
		MISSING_FILES=()
		for WAV in $WAVS; do
			if [ -f $DIR/$WAV.wav ]; then
				FILESIZE=$( wc -c $DIR/$WAV.wav | awk '{print $1}' )
				if [ $FILESIZE -gt $MAX_FILESIZE ]; then
					MAX_FILESIZE=$FILESIZE
				fi
			else
				MISSING=$(( $MISSING + 1 ))
				MISSING_FILES+=($DIR/$WAV.wav)
			fi
		done
		if [ $MISSING == 0 ]; then
			echo "$( du -hs $DIR | awk '{print $1}' ) - OK"
		else
			echo "$MISSING missing file(s)"
			for FILE in ${MISSING_FILES[@]}; do
				echo "    Missing file: $FILE"
			done
		fi
	fi
done

MAX_FILESIZE=$(( ($MAX_FILESIZE + 1023) / 1024 ))
echo "Maximum voice file: $MAX_FILESIZE kB"

