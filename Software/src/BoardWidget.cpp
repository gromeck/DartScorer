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
#include "BoardWidget.h"

int BoardWidget::scaleDownRadius(double radius)
{
	//LOG_DEBUG("scaleDownRadius(%f)",radius);
	return radius / board->BoardRadius * maxRadius;
}

int BoardWidget::scaleUpRadius(int radius)
{
	//LOG_DEBUG("scaleUpRadius(%d)",radius);
	return (double) radius /maxRadius * board->BoardRadius;
}

void BoardWidget::pie(double radius,int color)
{
	pie(radius,color,0,360);
}

void BoardWidget::pie(double radius,int color,double a1,double a2)
{
	int r = scaleDownRadius(radius);

	//LOG_DEBUG("radius=%.2f  color=%d  a1=%.2f  a2=%.2f  r=%d",radius,color,r);
	//LOG_DEBUG("drawing pie(x=%d,y=%d,w=%d,h=%d,a1=%.2f,a2=%.2f)",centerX - r,centerY - r,2 * r,2 * r,a1,a2);
	fl_color(color);
	fl_pie(centerX - r,centerY - r,2 * r,2 * r,a1,a2);
}

void BoardWidget::arc(double radius,int color)
{
	int r = scaleDownRadius(radius);

	//LOG_DEBUG("radius=%f  color=%d  r=%d",radius,color,r);
	fl_color(color);
	fl_arc(centerX - r,centerY - r,2 * r,2 * r,0,360);
}

BoardWidget::BoardWidget(int x, int y, int w, int h, const char *label) : Fl_Box::Fl_Box(x, y, w, h, label)
{
	/*
	**	get the instance for the board itself
	*/
	board = new Board;
}

void BoardWidget::cancelScoreHandler(void)
{
	scoreHandler = NULL;
}

void BoardWidget::setScoreHandler(void (*scoreHandler)(int factor,int value))
{
	this->scoreHandler = scoreHandler;
}

int BoardWidget::handle(int event)
{
	int rc = 0;

	//LOG_DEBUG("%s(%d)",std::string(fl_eventnames[event],event));

	switch (event) {
		case FL_RELEASE:
			/*
			**	user release the button (after a press) on a target
			*/
			//LOG_DEBUG("%s(%d)  x=%d  y=%d  button=%d",std::string(fl_eventnames[event]),event, Fl::event_x(),Fl::event_y(), Fl::event_button());
			double x = scaleUpRadius(Fl::event_x() - centerX);
			double y = scaleUpRadius(Fl::event_y() - centerY);
			int factor = 0;
			int value = 0;
			board->CoordinatesToPoints(x,y,&factor,&value);
			//LOG_DEBUG("x=%.2f  y=%.2f  factor=%d  value=%d  points=%s",x,y,factor,value,board->Points2String(factor,value));
			if (scoreHandler)
				(*scoreHandler)(factor,value);
			break;
	}
	return rc;
}

void BoardWidget::draw(void)
{
	// set clipping to the widget area
	fl_push_clip(x(),y(),w(),h());

	// draw the background
	fl_draw_box(FL_FLAT_BOX,x(),y(),w(),h(),BackgroundColor);

	/*
	**	compute the center
	*/
	maxRadius = std::min(w(),h()) / 2;
	centerX = x() + w() / 2;
	centerY = y() + h() / 2;
	//LOG_DEBUG("maxRadius=%d  centerX=%d  centerY=%d",maxRadius,centerX,centerY);

	fl_line_style(FL_SOLID,0);

	/*
	**	draw the board
	*/
	pie(board->BoardRadius,BoardColor);
	
	/*
	**	draw the double segments
	*/
	int font_size = (scaleDownRadius(board->BoardRadius) - scaleDownRadius(board->DoubleRadius)) / 2;

	for (int seg = 0;seg < board->Segments;seg++) {
		int angle = board->SegmentAngleStart + seg * board->SegmentAngle;

		pie(board->DoubleRadius,(seg % 2) ? EvenColor : OddColor,angle,angle + board->SegmentAngle);
		pie(board->OuterSingleRadius,(seg % 2) ? BrightColor : DarkColor,angle,angle + board->SegmentAngle);
		pie(board->TrippleRadius,(seg % 2) ? EvenColor : OddColor,angle,angle + board->SegmentAngle);
		pie(board->InnerSingleRadius,(seg % 2) ? BrightColor : DarkColor,angle,angle + board->SegmentAngle);

		std::string value = std::to_string(board->Values[seg]);
		fl_font(ValueFont,font_size);
		fl_color(ValueColor);
		double rad = (angle - board->SegmentAngleStart) * M_PI / -180;
		int radius = scaleDownRadius((board->BoardRadius + board->DoubleRadius) / 2);
		int x = centerX + (double) cos(rad) * radius;
		int y = centerY + (double) sin(rad) * radius;
		fl_draw(value.c_str(),x - fl_width(value.c_str()) / 2,y + fl_height() / 2);
		//fl_color(FL_RED);
		//fl_pie(x - 5,y - 5,10,10,0,360);
	}

	/*
	**	draw the spider
	*/
	arc(board->DoubleRadius,SpiderColor);
	arc(board->OuterSingleRadius,SpiderColor);
	arc(board->TrippleRadius,SpiderColor);
	arc(board->InnerSingleRadius,SpiderColor);
	fl_color(SpiderColor);
	for (int seg = 0;seg < board->Segments;seg++) {
		int angle = board->SegmentAngleStart + seg * board->SegmentAngle;

		double rad = angle * M_PI / -180;
		int radius = scaleDownRadius(board->DoubleRadius);
		int x = centerX + (double) cos(rad) * radius;
		int y = centerY + (double) sin(rad) * radius;
		fl_line(centerX,centerY,x,y);
	}

	/*
	**	draw the bull & spider
	*/
	pie(board->BullRadius,EvenColor);
	arc(board->BullRadius,SpiderColor);

	/*
	**	draw the bulls eye & spider
	*/
	pie(board->BullsEyeRadius,OddColor);
	arc(board->BullsEyeRadius,SpiderColor);

	// pop the clipping again
	fl_pop_clip();
}

BoardWidget::~BoardWidget()
{
}/**/
