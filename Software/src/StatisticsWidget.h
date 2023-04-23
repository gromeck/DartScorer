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
#ifndef __STATISTICSWIDGET_H__
#define __STATISTICSWIDGET_H__

#include "common.h"
#include "Statistics.h"

class StatisticsWidget : public Fl_Box
{
	private:
		const int BackgroundColor = 30;

		const int AxisYFontSize = 20;

		const int BorderLeft = AxisYFontSize * 3;
		const int BorderRight = BorderLeft;
		const int BorderTop = AxisYFontSize;
		const int BorderBottom = BorderTop;

		const int AxisXLineWidth = 1;
		const int AxisXLineStyle = FL_DASH;
		const int AxisXLineColor = FL_WHITE;

		const int AxisYLineWidth = 3;
		const int AxisYLineStyle = FL_SOLID;
		const int AxisYLineColor = FL_WHITE;

		const int AxisYMarkerColor = FL_WHITE;
		const int AxisYMarkerWidth = 1;
		const int AxisYMarkerLength = BorderLeft / 10;
		const int AxisYFontFace = FL_HELVETICA;

		const int DataDotRadius = 4;
		const int DataLineWidth = 4;

		int DataAColor = FL_RED;
		int DataBColor = FL_YELLOW;

		int type = Statistics::TypeUnknown;
		std::vector<Statistics> dataA;
		std::vector<Statistics> dataB;

		double dataMinimum = Statistics::Minimum;
		double dataMaximum = Statistics::Maximum;
		double dataRange = Statistics::Maximum - Statistics::Minimum;
		int dataSize;

		void findMinMax(void);
		int mapNtoX(int n);
		int mapValueToY(double average);

	public:
		StatisticsWidget(int x,int y,int w,int h,int type);
		~StatisticsWidget();

		void clearData(void) { clearDataA(); clearDataB(); };
		void clearDataA(void) { this->dataA.clear(); };
		void clearDataB(void) { this->dataB.clear(); };

		void setType(int type);
		void setColors(Fl_Color colorA,Fl_Color colorB);

		void setDataA(std::vector<Statistics>& data) { this->dataA = data; findMinMax(); }
		void setDataB(std::vector<Statistics>& data) { this->dataB = data; findMinMax(); }

		void draw(void);
};

#endif

/**/
