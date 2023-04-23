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
#include "ScoreInputUI.h"

void ScoreInputUI::setFactor(int factor)
{
	LOG_DEBUG("factor=%d",factor);
}

void ScoreInputUI::triggerPoints(int factor,int value)
{
	int points = factor * value;

	LOG_DEBUG("factor=%d  value=%d  points=%d",factor,value,points);

	if (scoreHandler)
		(*scoreHandler)(factor,value);
}

void ScoreInputUI::cancelScoreHandler(void)
{
	scoreHandler = NULL;
}

void ScoreInputUI::setScoreHandler(void (*scoreHandler)(int factor,int value))
{
	this->scoreHandler = scoreHandler;
}

ScoreInputUI::~ScoreInputUI()
{
	LOG_DEBUG("-");
}/**/
