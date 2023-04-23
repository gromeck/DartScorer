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
#include "common.h"
#include "Config.h"
#include "Database.h"
#include "DartScorer.h"
#include "Voice.h"
#include "camera.h"

using Poco::ConsoleChannel;
using Poco::Logger;

/*
**	setup logging via Poco
*/
ConsoleChannel *_channel = new ConsoleChannel();
Logger &_logger = Logger::create(PACKAGE_NAME,_channel,Poco::Message::PRIO_ERROR);

bool _shutdown = false;
bool _debug = false;

/*
**  the shutdown handler sets <_shutdown> to non-zero
*/
static void shutdown_handler(int sig)
{
	printf(PACKAGE_NAME ": received signal %d (%s) -- shutting down\n",sig,strsignal(sig));
	_shutdown = true;
}

/*
**	print the usage
*/
static void usage(const char *argv0)
{
	fprintf(stderr,"Usage: %s [options]\n",argv0);
	fprintf(stderr,"Options:\n");
	fprintf(stderr," -V\n --version\n"
					"          print version number an exit\n");
	fprintf(stderr," -?\n --help\n"
					"          show this help\n");
	fprintf(stderr," -v\n --verbose\n"
					"          be more verbose,\n"
					"          repeating the option increases the verbosity level.\n");
	fprintf(stderr," -f\n --fullscreen\n"
					"          open the GUI in fullscreen mode\n");
	fprintf(stderr," -a <darts per second>\n --autoplay <darts per second>\n"
					"          run in autoplay mode -- useful for regression testing\n"
					"          <darts per second> specifies the speed in darts per second,\n"
					"          where 0 means to disable the autoplay feature.\n");
	exit(-1);
}

/*
**	some globals
*/
Config *_config = NULL;
Database *_database = NULL;
Voice *_voice = NULL;
DartScorer *_dartScorer = NULL;

int main(int argc,char *argv[])
{
	int c,n,m;
	int logLevel = Poco::Message::PRIO_WARNING;
	int fullscreen = false;
	int autoplay = false;

	char short_options[BUFSIZ];
	struct option long_options[] = {
		{ "version",	0,	0,	'V' },
		{ "help",		0,	0,	'h' },
		{ "fullscreen",	0,	0,	'f' },
		{ "autoplay",	1,	0,	'a' },
		{ "verbose",	0,	0,	'v' },
		{ NULL,			0,	0,	0	},
	};

	/*
	**	setup the short option string
	*/
	for (n = m = 0;long_options[n].name;n++) {
		short_options[m++] = long_options[n].val;
		if (long_options[n].has_arg)
			short_options[m++] = ':';
	}
	short_options[m++] = '\0';

	while ((c = getopt_long(argc,argv,short_options,long_options,NULL)) >= 0) {
		switch (c) {
			case 'V':	/*
						**	print version and exit
						*/
						printf("*** " PACKAGE_NAME " Version " PACKAGE_VERSION " ***\n");
						printf("(c) 2023 by Christian Lorenz\n");
						exit(0);
						break;
			case '?':
			case 'h':	/*
						**	print the usage
						*/
						goto usage;
						break;
			case 'f':	/*
						**	run in fullscreen mode
						*/
						fullscreen = true;
						break;
			case 'a':	/*
						**	autoplay
						*/
						autoplay = atoi(optarg);
						break;
			case 'v':	/*
						**	be verbose
						*/
						logLevel++;
						break;
			default:	/*
						**	usage
						*/
						LOG_ERROR("unknown option %c",c);
			usage:		usage(argv[0]);
						exit(-1);
						break;
		}
	}

	/*
	**	set the log level
	*/
	_logger.setLevel(logLevel);

	/*
	**	init the l18n
	*/
	setlocale(LC_ALL,"");
	bindtextdomain(PACKAGE,LOCALEDIR);
	textdomain(PACKAGE);

	/*
	**	set signal handlers
	*/
	LOG_INFO("installing signal handlers");
	signal(SIGHUP,SIG_IGN);
	signal(SIGINT,shutdown_handler);
	signal(SIGQUIT,shutdown_handler);
	signal(SIGABRT,shutdown_handler);
	signal(SIGTERM,shutdown_handler);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGUSR2,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);
	signal(SIGTRAP,SIG_IGN);
	signal(SIGALRM,SIG_IGN);
	signal(SIGURG,SIG_IGN);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGVTALRM,SIG_IGN);

	/*
	**	initialize the cameras
	*/
	//camera_init();

	/*
	**	setup the configuration
	*/
	_config = new Config();

	/*
	**	setup the database
	*/
	_database = new Database();

	/*
	**	setup the voice
	*/
	_voice = new Voice();
	_voice->setVoice(_config->getConfigString(ConfigKeys::OptionsCallerVoice).c_str());

	/*
	**	create the UI
	*/
	_dartScorer = new DartScorer(fullscreen,autoplay);

	_dartScorer->run();

	/*
	**	enforce other threads to terminate
	*/
	_shutdown = true;

#if 0
//camera_calibration();

	/*
	**	create the combined window
	*/
	Mat image(Size(CAPTURE_WIDTH,CAMS * CAPTURE_HEIGHT),CV_8UC3);
    namedWindow(PACKAGE_NAME,true);

    for(;;) {
		for (int cam = 0;cam < _cams;cam++) {
			/*
			**	get the frame
			*/
			camera_get_frame(cam);

			/*
			**	make the image greyscale
			*/
			//cv::cvtColor(_camera[cam].frame,_camera[cam].frame,cv::COLOR_BGR2GRAY);

			/*
			**	add text to the image
			*/
			cv::rectangle(_camera[cam].frame,cv::Point(0,0),cv::Point(_camera[cam].frame.cols,25), CV_RGB(255,255,255),-1);
			sprintf(text,"CAM #%d: %dx%dpx",cam,_camera[cam].frame.cols,_camera[cam].frame.rows);
			cv::putText(_camera[cam].frame,text,Point(10,20),cv::FONT_HERSHEY_PLAIN,1,CV_RGB(0,0,0),1);

			/*
			**	display this image
			*/
			if (_showCamWindows)
				imshow(_camera[cam].windowTitle,_camera[cam].frame);
		}
 
 		/*
		**	concat the images
		*/
		if (_cams == CAMS)
			vconcat(_camera[0].frame,_camera[1].frame,image);
		else
			image = _camera[0].frame;

		/*
		**	display the resulting image
		*/
		imshow(PACKAGE_NAME,image);

		/*
		**	wait for user interaction
		*/
        char c = (char) waitKey(10);
        if (c == 27 || c == 'q')
            break;
		switch (c) {
			case 'c':
				camera_calibration();
				break;
		}
    }
#endif
}/**/
