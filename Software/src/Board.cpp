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
#include "Board.h"

Board::Board()
{
	LOG_DEBUG("-");
}

/*
**	convert coordinates in mm to the value of the boards segments
*/
int Board::CoordinatesToPoints(const double x,const double y,int *factor,int *value)
{
	double radius = sqrt(x * x + y * y);
	double angle = 0;
	
	*factor = Points::SingleFactor;

	if (radius <= BullRadius) {
		if (radius <= BullsEyeRadius)
			*factor = Points::DoubleFactor;
		*value = Points::Bull;
	}
	else if (radius <= DoubleRadius) {
		if (radius > InnerSingleRadius && radius <= TrippleRadius)
			*factor = Points::TrippleFactor;
		else if (radius > OuterSingleRadius && radius <= DoubleRadius)
			*factor = Points::DoubleFactor;

		/*
		**	compute the segment
		*/
		angle = atan2(y,x) / M_PI * -180 + 360;
		angle = std::fmod(angle + SegmentAngleStart,360);
		*value = Values[(int) (angle / SegmentAngle)];
	}
	else
		*value = 0;

	LOG_DEBUG("[%5.2f/%5.2f] radius=%5.2f  angle=%5.2f  factor=%d  value=%d",x,y,radius,angle,*factor,*value);

	return *factor * *value;
}

Board::~Board()
{
}/**/
