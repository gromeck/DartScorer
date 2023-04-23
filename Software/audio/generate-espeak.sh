#!/bin/bash
#
#	list espeak voices
#		espeak --voices=en
#		espeak --voices=de
#

VOICE="espeak"

for LANG in EN DE; do
	mkdir -p $LANG-$VOICE
	cat $LANG.words | while read LINE; do
		LINE="$( echo $LINE | sed -e "s/^[ \t]*//g" -e "s/[ \t]*$//g" )"
		[ "$LINE" == "" ] && continue
		[ "${LINE:0:1}" == "#" ] && continue
		INDEX="$( echo $LINE | cut -f1 -d= )"
		WORDS="$( echo $LINE | cut -f2 -d= )"
		[ "$WORDS" == "" ] && WORDS=$INDEX

		WAVFILE="$LANG-$VOICE/$INDEX.wav"

		if [ ! -f $WAVFILE ] || [ $LANG.words -nt $WAVFILE ] || [ $0 -nt $WAVFILE ]; then
			echo "$INDEX=$WORDS"
			espeak \
				-v $LANG \
				-z \
				-a 200 \
				-p 150 \
				-w $WAVFILE \
				"$WORDS"
		fi
	done
done
