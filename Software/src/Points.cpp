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
#include "Points.h"

Points::Points()
{
	set(0,0);
}

bool Points::set(int factor,int value)
{
	if (validate(factor,value)) {
		this->factor = factor;
		if (!(this->value = value))
			this->factor = SingleFactor;
		return true;
	}
	this->factor = 0;
	this->value = 0;
	return false;
}

bool Points::setRandom(void)
{
	while (!set(SingleFactor + rand() % (TrippleFactor - SingleFactor + 1),value = rand() % (Bull + 1)))
		;
	return true;
}

bool Points::validate(int factor,int value)
{
	if (value >= 0 && value <= S20 && factor >= SingleFactor && factor <= TrippleFactor)
		return true;
	if (value == Bull && factor >= SingleFactor && factor <= DoubleFactor)
		return true;
	return false;
}

std::string Points::toString(int factor,int value)
{
	std::string s;

	if (factor == DoubleFactor || factor == TrippleFactor)
		s = (factor == TrippleFactor) ? "T" : "D";
	if (value == Bull)
		s+= _config->getConfigInt(ConfigKeys::OptionsShowBullAs25,1) ? std::to_string(value) : "Bull";
	else if (value >= 0 && value <= S20)
		s += std::to_string(value);

	return s;
}

Points::~Points()
{
}/**/
