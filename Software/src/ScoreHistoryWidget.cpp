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
#include "ScoreHistoryWidget.h"

ScoreHistoryWidget::ScoreHistoryWidget(int x,int y,int w,int h) : Fl_Box::Fl_Box(x,y,w,h,"")
{
	LOG_DEBUG("x=%d y=%d w=%d h=%d",x,y,w,h);

	setValue("");
}

void ScoreHistoryWidget::setValue(const char *value)
{
	strcpy(this->value,value);
	redraw();
}

void ScoreHistoryWidget::draw(void)
{
	LOG_DEBUG("-");

	fl_push_clip(x(),y(),w(),h());

	fl_font(labelfont(),labelsize());

	fl_draw_box(FL_FLAT_BOX,x(),y(),w(),h(),color());

	int tw = fl_width(this->value) + fl_width(" ");
	fl_color(labelcolor());
	fl_draw(this->value,x() + w() - tw,y() + h() - (h() - labelsize() + fl_descent()) / 2);

	fl_pop_clip();
}

ScoreHistoryWidget::~ScoreHistoryWidget()
{
	LOG_DEBUG("-");
}/**/
