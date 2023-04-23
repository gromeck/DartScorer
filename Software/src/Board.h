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
#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.h"
#include "Points.h"
#include "Turn.h"

class Board
{
	public:
		Board();
		~Board();

		/*
		**	board dimensions in mm
		*/
		const double BullsEyeRadius = 12.7 / 2;
		const double BullRadius = 31.8 / 2;
		const double TrippleRadius = 107.0;
		const double InnerSingleRadius = TrippleRadius - 8.0;
		const double DoubleRadius = 170.0;
		const double OuterSingleRadius = DoubleRadius - 8.0;
		const double BoardRadius = 240.0;

		/*
		**	segement width in degrees
		*/
		static const int Segments = 20;
		const double SegmentAngle = 360 / Segments;

		/*
		**	segment offset in degrees from the start
		**	0° is at 3 o'clock, degrees is turning counter-clockwise
		**
		**	the first segmenets starts with value 6 and starts
		**	at 9° (half segment width)
		*/
		const double SegmentAngleStart = SegmentAngle / 2;

		/*
		**	values
		*/
		const int Values[20] = {
				6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11, 8, 16, 7, 19, 3, 17, 2, 15, 10
			};

		int CoordinatesToPoints(const double x,const double y,int *factor,int *value);
};

#endif

/**/
