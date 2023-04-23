# DartScorer
Yet another application to manage the game of Steel Dart.
The application is written in C++ on base of the following libraries:

- PocoLib https://pocoproject.org/
- FLTK https://www.fltk.org/
- OpenCV https://opencv.org/
- PortAudio http://www.portaudio.com/

DartScorer supports

- X01 with different options
- a simple player management
- audio caller voice
- score input via keyboard, or bluetooth number keypad
- display during gameplay as you know it from TV
- translated frontend (translaters for other languages than EN and DE are welcome)


# Building DartScorer

To build DartScorer, install the following packages into your
build environment:

```
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
```

After that, run the script ./build.sh

You can run DartScorer in the directory Software/src by issuing the command:

```
./DartScorer
```

or simple install DartScorer by issuing

```
sudo make install
```

In a free-desktop compatible environment DarScorer will appear in the category of Games.
