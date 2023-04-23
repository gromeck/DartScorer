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
#include "Statistics.h"

Statistics::Statistics()
{
	set(-1,TypeUnknown,0,0);
}

bool Statistics::set(int pid,int type,double value,time_t timestamp)
{
	this->pid = pid;
	this->type = type;
	this->value = value;
	this->timestamp = timestamp;
	return true;
}

bool Statistics::set(int pid,int type,double value)
{
	return set(pid,type,value,time(NULL));
}

const char *Statistics::Type2Name(int type)
{
	switch (type) {
		case TypeAverage:
			return TypeAverageString;
			break;
		case TypeDartsToLeg:
			return TypeDartsToLegString;
			break;
		case TypeHighFinish:
			return TypeHighFinishString;
			break;
		case TypeHighScores:
			return TypeHighScoresString;
			break;
	}
	return TypeUnknownString;
}

int Statistics::Name2Type(const char *name)
{
	if (name) {
		if (!strcmp(name,TypeAverageString))
			return TypeAverage;
		if (!strcmp(name,TypeDartsToLegString))
			return TypeDartsToLeg;
		if (!strcmp(name,TypeHighFinishString))
			return TypeHighFinish;
		if (!strcmp(name,TypeHighScoresString))
			return TypeHighScores;
	}
	return TypeUnknown;
}

Statistics::~Statistics()
{
}/**/
