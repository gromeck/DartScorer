#!/bin/bash

FONT_URL="https://fonts.google.com/download?family=Roboto+Condensed"
ZIPFILE="/tmp/fonts.zip"
TMPDIR="/tmp/fonts"
FONTDIR="/usr/share/fonts/truetype/"

mkdir -p $TMPDIR

echo "Loading fonts from $FONT_URL to $ZIPFILE ..."
wget -q -O $ZIPFILE $FONT_URL
echo "Unzipping fonts from $ZIPFILE to $TMPDIR ..."
unzip -o $ZIPFILE -d $TMPDIR
echo "Installing fonts from $TMPDIR into $FONTDIR ..."
cp -p $TMPDIR/*.ttf /usr/share/fonts/truetype/

rm -rf $TMPDIR

fc-cache
