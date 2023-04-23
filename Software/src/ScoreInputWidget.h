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
#ifndef __SCOREINPUTWIDGET_H__
#define __SCOREINPUTWIDGET_H__

#include "common.h"
#include "Points.h"
#include "Config.h"
#include "ConfigKeys.h"

class ScoreInputWidget : public Fl_Box
{
	private:
		int factor = Points::SingleFactor;
		int value = -1;
		std::string text;
		bool error = false;

		int handle(int event);

		static void resetError(ScoreInputWidget *widget);

		static const int ScoreKeypad0 = 0;
		static const int ScoreKeypad1 = 1;
		static const int ScoreKeypad2 = 2;
		static const int ScoreKeypad3 = 3;
		static const int ScoreKeypad4 = 4;
		static const int ScoreKeypad5 = 5;
		static const int ScoreKeypad6 = 6;
		static const int ScoreKeypad7 = 7;
		static const int ScoreKeypad8 = 8;
		static const int ScoreKeypad9 = 9;
		static const int ScoreKeypadBull = 25;
		static const int ScoreKeypadSingle = -1;
		static const int ScoreKeypadDouble = -2;
		static const int ScoreKeypadTripple = -3;
		static const int ScoreKeypadClear = -4;
		static const int ScoreKeypadUndo = -5;

		bool scoreInputRaw(int key);
		bool scoreInput(int key);

		void *data = NULL;

		bool (*triggerOnScoreInput)(void *data,Points& points) = NULL;
		bool (*triggerOnScoreUndo)(void *data) = NULL;

		void setText(void);

	public:
		ScoreInputWidget(int x,int y,int w,int h);
		~ScoreInputWidget();

		void callbackOnScoreInput(bool (*handler)(void *data,Points& points),void *data);
		void callbackOnScoreUndo(bool (*handler)(void *data),void *data);

		void clear(void);
		void setError(void);
		void draw(void);
};

#endif

/**/
