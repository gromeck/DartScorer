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
#ifndef __SCOREHISTORYWIDGET_H__
#define __SCOREHISTORYWIDGET_H__

#include "common.h"

class ScoreHistoryWidget : public Fl_Box
{
	public:
		ScoreHistoryWidget(int x,int y,int w,int h);
		~ScoreHistoryWidget();

		void setValue(const char *value);

	private:
		char value[100];

		void draw(void);
};

#endif

/**/
