#!/bin/bash

aclocal \
&& autoheader \
&& automake --gnu --add-missing --copy \
&& autoconf --force \
&& ./configure $* \
&& make clean \
&& make -j4
