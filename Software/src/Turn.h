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
#ifndef __TURN_H__
#define __TURN_H__

#include "common.h"
#include "Points.h"

class Turn
{
	public:
		static const int DartsPerTurn = 3;
		static const int HighestPointsPerTurn = DartsPerTurn * Points::T20;

	private:
		int darts;
		int pointsTotal;

		Points points[DartsPerTurn];

		int _found;

	public:
		Turn();
		~Turn();

		void clear(void);

		bool push(Points& p);
		bool push(int factor,int value);
		bool pop(void);

		int getDarts(void);
		int getPoints(void);
		int getPoints(int dart);
		int getPointsLastDart(void);
		int getCommonDarts(Turn *comp);
		bool isLastDartDouble(void);
		Points *getPointsPoints(int dart);

		std::string toString(Turn *comp);
		std::string toString(void);
};

#endif

/**/
