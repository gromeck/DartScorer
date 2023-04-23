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
#ifndef __BOARDWIDGET_H__
#define __BOARDWIDGET_H__

#include "common.h"
#include "Board.h"

class BoardWidget : public Fl_Box
{
	private:
		Board *board;
		int maxRadius;
		int centerX;
		int centerY;

		const int BackgroundColor = FL_GRAY;
		const int BoardColor = FL_BLACK;
		const int DarkColor = FL_BLACK;
		const int BrightColor = 54;
		const int OddColor = 120;
		const int EvenColor = FL_GREEN + 2 * 8;
		const int SpiderColor = FL_GRAY;
		const int ValueFont = FL_HELVETICA;
		const int ValueColor = FL_WHITE;
		
		int scaleDownRadius(double radius);
		int scaleUpRadius(int radius);
		void pie(double radius,int color);
		void pie(double radius,int color,double a1,double a2);
		void arc(double radius,int color);
		virtual int handle(int event);
		void draw(void);
		void (*scoreHandler)(int factor,int value);

	public:
		BoardWidget(int x, int y, int w, int h, const char *label);
		~BoardWidget();

		void cancelScoreHandler(void);
		void setScoreHandler(void (*scoreHandler)(int factor,int value));
};

#endif

/**/
