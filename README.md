# DartScorer
Yet another application to manage the game of Steel Dart.

Install missing packages

apt -y install \
	wget \
	git \
	g++ \
	make \
	automake \
	libxss-dev \
	gettext \
	libpoco-dev \
	libfltk1.3-dev \
	libpocoutil70 \
	portaudio19-dev \
	v4l-conf v4l-utils \
	opencv-data \
	opencv-doc \
	libopencv-video-dev \
	libopencv-videoio-dev \
	libopencv-highgui-dev \
	libopencv-features2d-dev \
	cheese

List available camera devices:

v4l2-ctl --list-devices

NOTE: Use the HBV ones!
