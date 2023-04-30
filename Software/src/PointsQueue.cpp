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
#include "PointsQueue.h"

PointsQueue::PointsQueue()
{
	LOG_DEBUG("-");
}

void PointsQueue::print(const char *title)
{
	LOG_DEBUG("title=%s",std::string(title));

	for (auto points: queue)
		LOG_DEBUG("%s: score=%s",std::string(title),points.toString());
}

void PointsQueue::clear(void)
{
	LOG_DEBUG("-");

	queue.clear();
}

int PointsQueue::length(void)
{
	LOG_DEBUG("length=%d",(int) queue.size());

	return queue.size();
}

bool PointsQueue::push(Points& points)
{
	LOG_DEBUG("points=%s",points.toString());

	queue.push_back(points);
	return true;
}

bool PointsQueue::pop()
{
	LOG_DEBUG("-");

	return pop(NULL);
}

bool PointsQueue::pop(Points *points)
{
	LOG_DEBUG("points=%s",PTR2STR(points));

	if (queue.size() > 0) {
		if (points)
			*points = queue[queue.size() - 1];
		queue.pop_back();
		return true;
	}
	return false;
}

bool PointsQueue::nth(int n,Points *points)
{
	LOG_DEBUG("n=%d  points=%s",n,PTR2STR(points));

	if (n >= 0 && n < (int) queue.size()) {
		*points = queue[n];
		return true;
	}
	return false;
}

std::string PointsQueue::toString(std::string separator)
{
	LOG_DEBUG("separator=%s",separator);

	std::string history = "";

	for (int n = ((int) queue.size() > QueueStringLimit) ? queue.size() - QueueStringLimit : 0;n < (int) queue.size();n++) {
		if (n > 0)
			history += separator;
		history += queue[n].toString();
	}
	return history;
}

std::string PointsQueue::toStringReverse(std::string separator)
{
	LOG_DEBUG("separator=%s",separator);

	std::string history = "";

	for (int n = ((int) queue.size() > QueueStringLimit) ? QueueStringLimit : queue.size() - 1;n >= 0;n--) {
		history += queue[n].toString();
		if (n > 0)
			history += separator;
	}
	return history;
}

PointsQueue::~PointsQueue()
{
	LOG_DEBUG("-");
}/**/
