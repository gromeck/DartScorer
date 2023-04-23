#!/bin/bash

#
#	get the devices
#
#v4l2-ctl --list-devices | grep HBV | sed -e "s/^.*\(//" -e "s/\).*$//" | while read USBID; do

v4l2-ctl --list-devices | grep HBV | while read CAM; do
	NAME="$( echo $CAM | sed -e "s/ *(.*$//" )"
	USBID="$( echo $CAM | sed -e "s/^.*(//" -e "s/).*$//" )"
	echo "$NAME ($USBID)"
	v4l2-ctl --media-bus-info $USBID --list-devices | grep "/dev/video" | sed -e "s/^/    /"
done
