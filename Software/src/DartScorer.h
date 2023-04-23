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
#ifndef __DARTSCORER_H__
#define __DARTSCORER_H__

#include "common.h"
#include "GameUI.h"

class DartScorer {

	public:
		static int lengthOfNumber(int number) { int len = 0; while (number > 0) { len++; number /= 10; } return len; }
		static int max(int a,int b)	{ return (a > b) ? a : b; }

		DartScorer(bool fullscreen = false,int autoplay = 0);
		~DartScorer();

		void run(void);
		void shutdown(void);
		void autoplayPlay(void);
		double autoplayTimeout = 0.10;	// in seconds

	private:
	
		bool fullscreen = false;
		int autoplay = 0;

		static constexpr double voicePlaybackTimeout = 0.01;	// in seconds

		GameUI *gameUI = NULL;

		bool setFullscreen(void);
		bool setFullscreen(bool fullscreen);

		static void loadFonts(void);

		static int handleEvents(int event);
		static void voicePlaybackTimer(void *data);
		static void autoplayTimer(void *data);
};

extern DartScorer *_dartScorer;

#endif

/**/
