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
#include "DartScorer.h"
#include <X11/extensions/scrnsaver.h>

DartScorer::DartScorer(bool fullscreen,int autoplay)
{
	LOG_DEBUG("fullscreen=%b  autoplay=%d",fullscreen,autoplay);

	this->fullscreen = fullscreen;
	if ((this->autoplay = autoplay) > 0) {
		autoplayTimeout = 1.0 / autoplay;
	}
	else {
		autoplayTimeout = 0;
		autoplay = 0;
	}

	/*
	**	FLTK stuff
	*/
	loadFonts();
	Fl::add_handler(handleEvents);
	Fl::add_timeout(voicePlaybackTimeout,voicePlaybackTimer);
	if (autoplay)
		Fl::add_timeout(autoplayTimeout,autoplayTimer);

	gameUI = new GameUI(fullscreen,(autoplay > 0) ? true : false);
}

bool DartScorer::setFullscreen(void)
{
	return gameUI->setFullscreen();
}

void DartScorer::loadFonts(void)
{
	Fl::set_font(FL_HELVETICA,"Roboto Condensed");
	Fl::set_font(FL_HELVETICA_BOLD,"BRoboto Condensed");
}

int DartScorer::handleEvents(int event)
{
	if (event == FL_NO_EVENT)
		return false;

	LOG_DEBUG("event=%d(%s)",event,std::string(fl_eventnames[event]));
	if (event == FL_SHORTCUT) {
		LOG_DEBUG("event=%d(%s)  key=%d(0x%02?x)",event,std::string(fl_eventnames[event]),Fl::event_key(),Fl::event_key());
		switch (Fl::event_key()) {
			case XK_F11:
				LOG_DEBUG("toggle fullscreen mode");
				if (_dartScorer)
					_dartScorer->setFullscreen();
				return true;
		}
	}
	return false;
}

void DartScorer::voicePlaybackTimer(void *)
{
//	LOG_DEBUG("-");

	if (_voice)
		_voice->playback();
	if (_shutdown)
		_dartScorer->shutdown();

	Fl::repeat_timeout(DartScorer::voicePlaybackTimeout,voicePlaybackTimer);
}

void DartScorer::autoplayTimer(void *)
{
//	LOG_DEBUG("-");

	if (_dartScorer) {
		_dartScorer->autoplayPlay();
		Fl::repeat_timeout(_dartScorer->autoplayTimeout,autoplayTimer);
	}
}

void DartScorer::autoplayPlay(void)
{
//	LOG_DEBUG("-");
	if (autoplay)
		gameUI->autoplayPlay();
}

void DartScorer::run(void)
{
	LOG_DEBUG("-");

	XScreenSaverSuspend(fl_display,true);

	gameUI->window->show();
	Fl::run();

	XScreenSaverSuspend(fl_display,false);
}

void DartScorer::shutdown(void)
{
	LOG_DEBUG("-");

	while(Fl::first_window())
		delete Fl::first_window();
}

DartScorer::~DartScorer()
{
	LOG_DEBUG("-");

	delete gameUI;
}/**/
