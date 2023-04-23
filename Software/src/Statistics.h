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
#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "common.h"

class Statistics {
	public:
		static const int TypeUnknown = 0;
		static const int TypeAverage = 1;
		static const int TypeDartsToLeg = 2;
		static const int TypeHighFinish = 3;
		static const int TypeHighScores = 4;

		static const int TypeMin = TypeAverage;
		static const int TypeMax = TypeHighScores;

		static constexpr const double Minimum = 0;
		static constexpr const double Maximum = 180;

	private:
		int pid = -1;
		int type = TypeUnknown;
		double value = 0;
		time_t timestamp = 0;

	public:
		static constexpr const char *TypeUnknownString = "unknown";
		static constexpr const char *TypeAverageString = "Average";
		static constexpr const char *TypeDartsToLegString = "Darts To Leg";
		static constexpr const char *TypeHighFinishString = "High Finish";
		static constexpr const char *TypeHighScoresString = "High Scores";

	public:
		Statistics();
		Statistics(int pid,int type,double value) { set(pid,type,value); };
		Statistics(int pid,int type,double value,time_t timestamp) { set(pid,type,value,timestamp); };
		~Statistics();

		static const char *Type2Name(int type);
		static int Name2Type(const char *str);

		bool set(int pid,int type,double value,time_t timestamp);
		bool set(int pid,int type,double value);

		int getPid(void) { return pid; };
		int getType(void) { return type; };
		double getValue(void) { return value; };
		time_t getTimestamp(void) { return timestamp; };

};

#endif

/**/
