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
#ifndef __POINTS_H__
#define __POINTS_H__

#include "common.h"
#include "Config.h"
#include "ConfigKeys.h"

class Points {
	private:
		int factor = 0;
		int value = 0;

	public:
		static const int TrippleFactor = 3;
		static const int DoubleFactor = 2;
		static const int SingleFactor = 1;

		static const int S1 = 1;
		static const int S20 = 20;
		static const int T20 = TrippleFactor * S20;
		static const int Bull = 25;

		static constexpr const char *SeparatorNarrow = "\xE2\x80\x8A\xC2\xB7\xE2\x80\x8A";	// hairspace + middot + hairspace
		static constexpr const char *SeparatorWide = " \xC2\xB7 ";	// space + middot + space

		Points();
		Points(int factor,int value) : Points() { set(factor,value); };
		~Points();

		bool set(int factor,int value);
		bool setRandom(void);

		static bool validate(Points& points) { return validate(points.factor,points.value); };
		static bool validate(int factor,int value);

		int getFactor(void) { return factor; };
		int getValue(void) { return value; };
		int getPoints(void) { return factor * value; };

		std::string toString(void) { return Points::toString(factor,value); };
		static std::string toString(int factor,int value);

		inline bool operator==(Points& rhs) { return getFactor() == rhs.getFactor() && getValue() == rhs.getValue(); }
		inline bool operator!=(Points& rhs) { return !(*this == rhs); }
};

#endif

/**/
