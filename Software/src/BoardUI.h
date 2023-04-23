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
#ifndef __BOARDUI_H__
#define __BOARDUI_H__

#include "common.h"
#include "DartScorerGUI.h"

class BoardUI : public BoardGUI
{
	private:
		void (*scoreHandler)(int factor,int value);

	public:
		BoardUI();
		~BoardUI();

		void cancelScoreHandler(void);
		void setScoreHandler(void (*scoreHandler)(int factor,int value));

		void addPoints(int factor,int value);
};

#endif

/**/
