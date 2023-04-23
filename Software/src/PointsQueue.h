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
#ifndef __POINTSQUEUE_H__
#define __POINTSQUEUE_H__

#include "common.h"
#include "Points.h"

class PointsQueue
{
	private:
		std::vector<Points> queue;

		void print(const char *title);

		const char QueueStringLimit = 50;	// toString() will only extract this limit of last entries

	public:
		PointsQueue();
		~PointsQueue();

		void clear(void);

		int length(void);

		bool push(Points& points);

		bool pop(void);
		bool pop(Points *points);

		bool nth(int n,Points *points);

		std::string toString(void) { return toString(Points::SeparatorWide); };
		std::string toString(std::string separator);
};

#endif

/**/
