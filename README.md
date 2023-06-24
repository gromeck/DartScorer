# DartScorer :dart:
Yet another application to manage the game of Steel Dart

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-Welcome.png" height="450px">
</div>

&nbsp;

The application is written in C++ on base of the following libraries:

- PocoLib https://pocoproject.org/
- FLTK https://www.fltk.org/
- OpenCV https://opencv.org/
- PortAudio http://www.portaudio.com/

DartScorer :dart: supports:

- X01 with different options (set/leg based, double/straight out)
- a simple player management
- audio caller voice (different audio packages available -- visit https://github.com/gromeck/DartScorer-Voices)
- score input via keyboard or bluetooth number keypad -- with unlimited undo and redo
- checkout proposals
- display during gameplay as you know it from TV
- translated frontend (translaters for other languages than EN and DE are welcome)
- simple statistics (average, high scores & finishes, ...)

# Building DartScorer :dart:

To build DartScorer :dart:, install the following packages into your
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

You can run DartScorer :dart: in the directory Software/src by issuing the command:

```
./DartScorer
```

or simple install DartScorer :dart: by issuing

```
sudo make install
```

In a free-desktop compatible environment DartScorer :dart: will appear in the category of Games.


# Score input via keypad

I recommend a bluetooth keypad to use for manual score input. DartScorer :dart: supports
two different layouts which only differ in PC- or phone-like ordering of the numbers (0 to 9).

An unmodified keypad looks like this:

<div style="float:left;">
<img src="Hardware/Keypad/Keypad-Original.png" height="300px">
</div>


Get some white-on-black tape for our labeling machine and make it look like this:

<div style="float:left;">
<img src="Hardware/Keypad/Keypad-Modified-PC.png" height="300px">
</div>


Here is my keypad:

<div style="float:left;">
<img src="Hardware/Keypad/Keypad-Example.jpg" height="300px">
</div>

| Keypad Key  | Alternative Input | DartScorer :dart: Function |
| ----------- | ----------------- | -------------------------- |
| `0`         | `0`               | `0`                        |
| :           | :                 | :                          |
| `9`         | `9`               | `9`                        |
| `.`         | `b`               | `Bull`                     |
| `Enter`     | `s`               | `Sx` (Single)              |
| `+`         | `d`               | `Dx` (Double)              |
| `-`         | `t`               | `Tx` (Tripple)             |
| `*`         | `r`               | `Redo`                     |
| `Backspace` | `u`               | `Undo`                     |


DartScorer :dart: also offers two modes to enter the score.

The current input is show in the rightmost field at the screen bottom.

In general it was the goal to enter the score with as less key strokes
as possible.

## Score Input Mode `T..2..0`

In this input mode you enter the score as it is normally spoken,
first the factor (single, double, tripple), then the digits.
The single can be omitted. Bulls Eye is entered as `Dx`...`Bull`.

**Note:** in case of a score of 1 or 2 you enter `1` or respectivly `2`,
but DartScorer :dart: will wait for another digit (score could be
1 or 10 to 19 respectivly 2 or 20).

You have to hit `Sx` or wait for two seconds to close the input of
these scores.
The same is true for D1, D2, T1 and T2.



See the table below for examples.


## Score Input Mode `2..0..T`

In this input order you enter the score in the order digits, followed
by the factor (single, double, tripple).
Bulls Eye is entered as `Bull`...`Dx`.

See the table below for examples.


## Comparison and Examples

| Score        | Input Sequence for `T..2..0` | Input Sequence for `2..0..T` |
| ------------ | ---------------------------- | ---------------------------- |
| 0            | `0`                          | `0` ... `Sx`                 |
| **Singles**                                                                |
| 1            | `1` ... `Sx`                 | `1` ... `Sx`                 |
| 2            | `2` ... `Sx`                 | `1` ... `Sx`                 |
| 3            | `3`                          | `1` ... `Sx`                 |
| :            | :                            | :                            |
| 9            | `9`                          | `9` ... `Sx`                 |
| 10           | `1` ... `0`                  | `1` ... `0` ... `Sx`         |
| :            | :                            | :                            |
| 19           | `1` ... `9`                  | `1` ... `9` ... `Sx`         |
| 20           | `2` ... `0`                  | `2` ... `0` ... `Sx`         |
| **Doubles**                                                                |
| D1           | `Dx` ... `1` ... `Sx`        | `1` ... `Dx`                 |
| D2           | `Dx` ... `2` ... `Sx`        | `2` ... `Dx`                 |
| D3           | `Dx` ... `3`                 | `3` ... `Dx`                 |
| :            | :                            | :                            |
| D9           | `Dx` ... `9`                 | `9` ... `Dx`                 |
| D10          | `Dx` ... `1` ... `0`         | `1` ... `0` ... `Dx`         |
| :            | :                            | :                            |
| D19          | `Dx` ... `1` ... `9`         | `1` ... `9` ... `Dx`         |
| D20          | `Dx` ... `2` ... `0`         | `2` ... `0` ... `Dx`         |
| **Tripples** | analog                                                      |
| Bull         | `Bull`                       |`Bull` ... `Sx`               |
| Bulls Eye    | `Dx` ... `Bull`              |`Bull` ... `Dx`               |



# Some more Screenshots

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-ChoosePlayers.png" height="450px">
</div>

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-Options.png" height="450px">
</div>

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-ChoosePlayers.png" height="450px">
</div>

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-Game-X01-Options.png" height="450px">
</div>

<div style="float:left;">
<img src="Resources/Screenshots/DartScorer-Screenshot-Game-X01-Play.png" height="450px">
</div>
