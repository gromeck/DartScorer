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
#include "Turn.h"

Turn::Turn()
{
	clear();
}

void Turn::clear(void)
{
	pointsTotal = 0;
	darts = 0;
}

bool Turn::push(Points& p)
{
	if (darts < DartsPerTurn) {
		points[darts++] = p;
		pointsTotal += p.getPoints();
		return true;
	}
	return false;
}

bool Turn::push(int factor,int value)
{
	Points p(factor,value);

	return push(p);
}

bool Turn::pop(void)
{
	if (darts > 0) {
		pointsTotal -= points[--darts].getPoints();
		return true;
	}
	return false;
}

int Turn::getDarts(void)
{
	return darts;
}

int Turn::getPoints(void)
{
	return pointsTotal;
}

int Turn::getPoints(int dart)
{
	if (dart < darts)
		return points[dart].getPoints();

	return 0;
}

int Turn::getPointsLastDart(void)
{
	return getPoints(darts - 1);
}

bool Turn::isLastDartDouble(void)
{
	if (darts > 0)
		return points[darts - 1].getFactor() == Points::DoubleFactor;
	return false;
}

Points *Turn::getPointsPoints(int dart)
{
	if (dart >= 0 && dart < darts)
		return &points[dart];

	return NULL;
}

int Turn::getCommonDarts(Turn *comp)
{
	int commonDarts = 0;

	LOG_DEBUG("this=%s  comp=%s",this->toString(),(comp) ? comp->toString() : std::string("-"));
	for (int dart = 0;dart < darts && dart < comp->getDarts();dart++) {
		if (points[dart] != *comp->getPointsPoints(dart))
			break;
		commonDarts++;
	}
	LOG_DEBUG("this=%s  comp=%s  commonDarts=%d",this->toString(),(comp) ? comp->toString() : std::string("-"),commonDarts);
	return commonDarts;
}

std::string Turn::toString(Turn *comp)
{
	std::string s;

	for (int dart = (comp) ? getCommonDarts(comp) : 0;dart < darts;dart++) {
		if (s.length() > 0)
			s += Points::SeparatorNarrow;
		s += points[dart].toString();
	}
	return s;
}

std::string Turn::toString(void)
{
	return toString(NULL);
}

Turn::~Turn()
{
}/**/
