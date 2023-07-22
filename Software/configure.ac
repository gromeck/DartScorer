#
#	configure.ac
#
#	Copyright (c) 2020 by Christian Lorenz
#
#	Process this file with autoconf to produce a configure script.
#

AC_INIT([DartScorer], m4_esyscmd([ tr "\n" "-" <VERSION ; git describe --tags --always --dirty | tr -d "\n" ]) )
AC_PREFIX_DEFAULT(/usr)
AC_CONFIG_SRCDIR([.])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AC_PROG_CC
AC_PROG_CXX
AM_GNU_GETTEXT([external])

# static
AC_ARG_ENABLE(static,
		[  --enable-static         enable static build],
			       STATIC_ENABLED="$enableval",STATIC_ENABLED=no)
AM_CONDITIONAL(STATIC_ENABLED, test "$STATIC_ENABLED" = yes)

#######################################
#
#	CFLAGS & LIBS
#
if test "$STATIC_ENABLED" = yes; then
	CXXFLAGS="$CFLAGS --static"
	LDFLAGS="$LDFLAGS --static"
fi
POCOLIBS="-lPocoUtil -lPocoFoundation -lPocoNet -lPocoData -lPocoDataSQLite"
OPENCVLIBS="-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_features2d"
CXXFLAGS="$CXXFLAGS -g -ggdb -fno-strict-aliasing -fPIC -Wall"
CXXFLAGS="$CXXFLAGS -D_GNU_SOURCE"
CXXFLAGS="$CXXFLAGS -I/usr/include/opencv4"
LDFLAGS="$LDFLAGS -g -ggdb"
LIBS="$LIBS -lfltk -lfltk_images"
LIBS="$LIBS -lXss"
LIBS="$LIBS $OPENCVLIBS"
LIBS="$LIBS $POCOLIBS -lpthread"
LIBS="$LIBS -lportaudio"
if test "$STATIC_ENABLED" = yes; then
	LIBS="$LIBS -lXinerama -lXcursor -lxkbcommon -lXfixes -lXft -lX11-xcb -lfontconfig -lexpat -lfreetype -lpng16 -lXrender -lXext -lX11 -lxcb -lXss -lXau -lXdmcp -lm -ldl -lz -luuid -lpthread -pthread"
fi

#
#	find all Makefiles
#
AC_CONFIG_FILES([ Makefile src/Makefile po/Makefile.in images/Makefile audio/Makefile etc/Makefile ])
AC_OUTPUT
