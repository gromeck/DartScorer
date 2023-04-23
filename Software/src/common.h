/*

	DartScorer

	(c) 2023 by Christian.Lorenz@gromeck.de

	This file is part of DartScorer

    DartScorer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DartScorer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DartScorer.  If not, see <https://www.gnu.org/licenses/>.

*/
#ifndef __COMMON_H__
#define __COMMON_H__

/*
**	stdard stuff
*/
#include <assert.h>
#include <cstddef>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <libintl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <vector>

/*
**	FLTK stuff
*/
#include <FL/names.h>
#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Output.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

/*
**	Poco stuff
*/
#include <Poco/Channel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Environment.h>
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <Poco/Path.h>
#include <Poco/Util/XMLConfiguration.h>

/*
**	openCV stuff
*/
#include <opencv2/core/version.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

/*
**	local stuff
*/
#include "config.h"

/*
**	some i18n stuff
*/
#define _(String)	gettext(String)

#define PTR2STR(p)					Poco::NumberFormatter::format((void *) (p))

#define LOG_LOCATION				std::string(__FILE__ ":" ) + std::to_string(__LINE__) + std::string(":") + std::string(__func__) + std::string(":")

#define LOG_ERROR(args...)			poco_error(_logger,LOG_LOCATION + "ERROR: " + Poco::format(args,NULL))
#define LOG_WARNING(args...)		poco_warning(_logger,LOG_LOCATION + "WARNING: " + Poco::format(args,NULL))
#define LOG_NOTICE(args...)			poco_notice(_logger,LOG_LOCATION + "NOTICE: " + Poco::format(args,NULL))
#define LOG_INFO(args...)			poco_information(_logger,LOG_LOCATION + "INFO: " + Poco::format(args,NULL))
#define LOG_DEBUG(args...)			poco_information(_logger,LOG_LOCATION + "DEBUG: " + Poco::format(args,NULL))

extern bool _shutdown;
extern bool _debug;

extern Poco::ConsoleChannel *_channel;
extern Poco::Logger &_logger;

#endif

/**/
