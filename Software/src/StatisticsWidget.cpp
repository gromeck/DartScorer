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
#include "StatisticsWidget.h"

StatisticsWidget::StatisticsWidget(int x,int y,int w,int h,int type) : Fl_Box::Fl_Box(x,y,w,h,"")
{
	LOG_DEBUG("x=%d  y=%d  w=%d  h=%d",x,y,w,h);

	setType(type);
}

void StatisticsWidget::setType(int type)
{
	LOG_DEBUG("type=%d",type);

	this->type = type;
	clearData();
}

void StatisticsWidget::setColors(Fl_Color colorA,Fl_Color colorB)
{
	this->DataAColor = colorA;
	this->DataBColor = colorB;
}

void StatisticsWidget::findMinMax(void)
{
	dataMinimum = Statistics::Maximum;
	dataMaximum = Statistics::Minimum;

	dataSize = (dataA.size() > dataB.size()) ? dataA.size() : dataB.size();
	for (int n = 0;n < (int) dataA.size();n++) {
		double value = dataA[n].getValue();

		if (value < dataMinimum)
			dataMinimum = value;
		if (value > dataMaximum)
			dataMaximum = value;
		LOG_DEBUG("A [n=%d]  value=%5.1f  time=%lu",n,dataA[n].getValue(),(unsigned long) dataA[n].getTimestamp());
	}
	for (int n = 0;n < (int) dataB.size();n++) {
		double value = dataB[n].getValue();

		if (value < dataMinimum)
			dataMinimum = value;
		if (value > dataMaximum)
			dataMaximum = value;
		LOG_DEBUG("B [n=%d]  value=%5.1f  time=%lu",n,dataB[n].getValue(),(unsigned long) dataB[n].getTimestamp());
	}
	dataRange = dataMaximum - dataMinimum;
	LOG_DEBUG("values=%d  dataMinimum=%5.2f  dataMaximum=%5.2f  dataRange=%5.2f",
			dataSize,dataMinimum,dataMaximum,dataRange);
	dataMinimum = floor(dataMinimum / 10 - 1) * 10;
	dataMaximum = ceil(dataMaximum / 10 + 1) * 10;
	dataRange = dataMaximum - dataMinimum;
	LOG_DEBUG("values=%d  dataMinimum=%5.2f  dataMaximum=%5.2f  dataRange=%5.2f",
			dataSize,dataMinimum,dataMaximum,dataRange);
}

int StatisticsWidget::mapNtoX(int n)
{
	int x = BorderLeft;

	if (dataSize > 1)
		x += (w() - BorderLeft - BorderRight) * n / (dataSize - 1);
	if (x > w())
		x = w();
	return this->x() + w() - x;	// as the values are descending, we have to mirror/turn the x
}

int StatisticsWidget::mapValueToY(double value)
{
	int y = h() - BorderBottom;

	if (dataRange > 0)
		y -= (h() - BorderTop - BorderBottom) * (value - dataMinimum) / dataRange;
	if (y > h())
		y = h();
	return this->y() + y;
}

void StatisticsWidget::draw(void)
{
	LOG_DEBUG("-");

	fl_push_clip(x(),y(),w(),h());

	fl_font(labelfont(),labelsize());

	fl_draw_box(FL_FLAT_BOX,x(),y(),w(),h(),BackgroundColor);

	/*
	**	draw the y axis
	*/
	fl_color(AxisYLineColor);
	fl_line_style(AxisYLineStyle,AxisYLineWidth);
	fl_yxline(x() + BorderLeft,y(),y() + h() - 1);
	fl_yxline(x() + w() - BorderRight,y(),y() + h() - 1);

	for (int value = dataMinimum;value <= dataMaximum;value += 10) {
		int y = mapValueToY(value);

		// the X lines/grid
		fl_color(AxisXLineColor);
		fl_line_style(AxisXLineStyle,AxisXLineWidth);
		fl_xyline(x() + BorderLeft,y,x() + w() - BorderRight);
		
		// the markers
		fl_color(AxisYMarkerColor);
		fl_line_style(FL_SOLID,AxisYMarkerWidth);
		fl_xyline(x() + BorderLeft - AxisYMarkerLength,y,x() + BorderLeft + AxisYMarkerLength);
		fl_xyline(x() + w() - BorderRight - AxisYMarkerLength,y,x() + w() - BorderRight + AxisYMarkerLength);

		// the marker labels
		fl_font(AxisYFontFace,AxisYFontSize);
		int labelW = fl_width(Poco::format("%d",value).c_str());
		fl_draw(Poco::format("%d",value).c_str(),x() + BorderLeft / 2 - labelW / 2,y - fl_descent() / 2 + AxisYFontSize / 2);
		fl_draw(Poco::format("%d",value).c_str(),x() + w() - BorderRight / 2 - labelW / 2,y - fl_descent() / 2 + AxisYFontSize / 2);
	}

	/*
	**	draw the data lines
	*/
	fl_line_style(FL_SOLID,DataLineWidth);
	int lastX,lastY;
	for (int n = 0;n < (int) dataA.size();n++) {
		int x = mapNtoX(n);
		int y = mapValueToY(dataA[n].getValue()); 

		if (n > 0) {
			fl_color(DataAColor);
			fl_line(x,y,lastX,lastY);
		}
		lastX = x;
		lastY = y;
	}
	for (int n = 0;n < (int) dataB.size();n++) {
		int x = mapNtoX(n);
		int y = mapValueToY(dataB[n].getValue()); 

		if (n > 0) {
			fl_color(DataBColor);
			fl_line(x,y,lastX,lastY);
		}
		lastX = x;
		lastY = y;
	}

	/*
	**	draw the data dots
	*/
	for (int n = 0;n < (int) dataA.size();n++) {
		int x = mapNtoX(n);
		int y = mapValueToY(dataA[n].getValue()); 

		fl_color(DataAColor);
		fl_pie(x - DataDotRadius,y - DataDotRadius,DataDotRadius * 2,DataDotRadius * 2,0,360);
	}
	for (int n = 0;n < (int) dataB.size();n++) {
		int x = mapNtoX(n);
		int y = mapValueToY(dataB[n].getValue()); 

		fl_color(DataBColor);
		fl_pie(x - DataDotRadius,y - DataDotRadius,DataDotRadius * 2,DataDotRadius * 2,0,360);
	}

	fl_pop_clip();
}

StatisticsWidget::~StatisticsWidget()
{
	LOG_DEBUG("-");
}/**/
