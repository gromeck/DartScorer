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
#ifndef __SCOREINPUTUI_H__
#define __SCOREINPUTUI_H__ 1

#include "common.h"
#include "DartScorerGUI.h"

class ScoreInputUI : public ScoreInputGUI
{
	private:
		void setFactor(int factor);
		void triggerPoints(int factor,int value);

		void (*scoreHandler)(int factor,int value);

	public:
		ScoreInputUI();
		~ScoreInputUI();

		void cancelScoreHandler(void);
		void setScoreHandler(void (*scoreHandler)(int factor,int value));
};

#endif

/**/
