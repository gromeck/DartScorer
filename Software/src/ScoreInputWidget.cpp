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
#include "ScoreInputWidget.h"

ScoreInputWidget::ScoreInputWidget(int x,int y,int w,int h) : Fl_Box::Fl_Box(x,y,w,h,"")
{
	LOG_DEBUG("x=%d y=%d w=%d h=%d",x,y,w,h);

	clear();
}

int ScoreInputWidget::handle(int event)
{
	// LOG_DEBUG("event=%d name=%s",event,std::string(fl_eventnames[event]));
	int rc = false;

	if (event == FL_KEYUP) {
		LOG_DEBUG("event=%d  %s: key=%d(0x%02?x)",event,std::string(fl_eventnames[event]),Fl::event_key(),Fl::event_key());
		rc = scoreInputRaw(Fl::event_key());
		LOG_DEBUG("event=%d  scoreInputRaw()=%d",event,rc);
	}
	return (rc) ? true : Fl_Box::handle(event);
}

void ScoreInputWidget::callbackOnScoreInput(bool (*handler)(void *data,Points& points),void *data)
{
	LOG_DEBUG("handler=%s  data=%s",PTR2STR(handler),PTR2STR(data));
	this->triggerOnScoreInput = handler;
	this->data = data;
}

void ScoreInputWidget::callbackOnScoreUndo(bool (*handler)(void *data),void *data)
{
	LOG_DEBUG("handler=%s  data=%s",PTR2STR(handler),PTR2STR(data));
	this->triggerOnScoreUndo = handler;
	this->data = data;
}

/*
**	handle the raw input of keys
*/
bool ScoreInputWidget::scoreInputRaw(int key)
{
	if (_config->getConfigInt(ConfigKeys::OptionsKeypadLayoutPhone,0)) {
		// if keypad phone layout, swap row 7 to 9 with 1 to 3
		switch (key) {
			case XK_KP_7: key = XK_KP_1; break;
			case XK_KP_8: key = XK_KP_2; break;
			case XK_KP_9: key = XK_KP_3; break;
			case XK_KP_1: key = XK_KP_7; break;
			case XK_KP_2: key = XK_KP_8; break;
			case XK_KP_3: key = XK_KP_9; break;
		}
	}
	switch (key) {
		case '0':
		case XK_KP_0:			return scoreInput(ScoreKeypad0);
		case '1':
		case XK_KP_1:			return scoreInput(ScoreKeypad1);
		case '2':
		case XK_KP_2:			return scoreInput(ScoreKeypad2);
		case '3':
		case XK_KP_3:			return scoreInput(ScoreKeypad3);
		case '4':
		case XK_KP_4:			return scoreInput(ScoreKeypad4);
		case '5':
		case XK_KP_5:			return scoreInput(ScoreKeypad5);
		case '6':
		case XK_KP_6:			return scoreInput(ScoreKeypad6);
		case '7':
		case XK_KP_7:			return scoreInput(ScoreKeypad7);
		case '8':
		case XK_KP_8:			return scoreInput(ScoreKeypad8);
		case '9':
		case XK_KP_9:			return scoreInput(ScoreKeypad9);

		case 'b':
		case XK_KP_Separator:	return scoreInput(ScoreKeypadBull);

		case 's':
		case XK_KP_Enter:		return scoreInput(ScoreKeypadSingle);
		case 'd':
		case XK_KP_Add:			return scoreInput(ScoreKeypadDouble);
		case 't':
		case XK_KP_Subtract:	return scoreInput(ScoreKeypadTripple);

		case 'c':
		case XK_BackSpace:		return scoreInput(ScoreKeypadClear);
		case 'u':
		case XK_KP_Multiply:	return scoreInput(ScoreKeypadUndo);
	}
	return false;
}

bool ScoreInputWidget::scoreInput(int key)
{
	LOG_DEBUG("key=%d",key);

	if (_config->getConfigInt(ConfigKeys::OptionsKeypadPointsFactor,1)) {
		/*
		**	input method: points before factor
		*/
		if (key >= ScoreKeypad0 && key <= ScoreKeypad9) {
			if (value < 0)
				value = 0;
			value = value * 10 + key;
			if (value != Points::Bull && (value < 0 || value > Points::S20)) {
				value = -1;
				setError();
			}
		}
		else if (key == ScoreKeypadBull) {
			value = Points::Bull;
		}
		else {
			switch (key) {
				case ScoreKeypadSingle:
					if (triggerOnScoreInput) {
						Points points(Points::SingleFactor,value);
						if ((*triggerOnScoreInput)(data,points))
							clear();
						else
							setError();
					}
					break;
				case ScoreKeypadDouble:
					if (triggerOnScoreInput) {
						Points points(Points::DoubleFactor,value);
						if ((*triggerOnScoreInput)(data,points))
							clear();
						else
							setError();
					}
					break;
				case ScoreKeypadTripple:
					if (triggerOnScoreInput) {
						Points points(Points::TrippleFactor,value);
						if ((*triggerOnScoreInput)(data,points))
							clear();
						else
							setError();
					}
					break;
				case ScoreKeypadUndo:
					if (triggerOnScoreUndo) {
						if ((*triggerOnScoreUndo)(data))
							clear();
						else
							setError();
					}
					break;
				default:
				case ScoreKeypadClear:
					clear();
					break;
			}
			value = -1;
		}
		factor = Points::SingleFactor;
		setText();

		return true;
	}
	else {
		/*
		**	input method: factor before points
		*/
		bool emit = false;

		if (key == ScoreKeypadBull) {
			value = Points::Bull;
			if (factor == Points::TrippleFactor)
				setError();
		}
		else if (key >= ScoreKeypad0 && key <= ScoreKeypad9) {
			if (value < 0)
				value = 0;
			value = value * 10 + key;
			if (value != Points::Bull && (value < 0 || value > Points::S20))
				setError();
			if (value == Points::Bull && factor == Points::TrippleFactor)
				setError();
		}
		else {
			switch (key) {
				case ScoreKeypadSingle:
				case ScoreKeypadDouble:
				case ScoreKeypadTripple:
					if (value >= 0)
						emit = true;
					break;
			}
		}
		LOG_DEBUG("key=%d  factor=%d  value=%d  emit=%d",key,factor,value,emit);
		if (emit || value == 0 || value >= 3) {
			// emit the value
			LOG_DEBUG("key=%d  emitting  factor=%d  value=%d  emit=%d",key,factor,value,emit);
			Points points(factor,value);
			if ((*triggerOnScoreInput)(data,points))
				clear();
			else
				setError();
		}
		switch (key) {
			case ScoreKeypadSingle:
				factor = Points::SingleFactor;
				break;
			case ScoreKeypadDouble:
				factor = Points::DoubleFactor;
				break;
			case ScoreKeypadTripple:
				factor = Points::TrippleFactor;
				break;
			case ScoreKeypadUndo:
				if (triggerOnScoreUndo) {
					if ((*triggerOnScoreUndo)(data))
						clear();
					else
						setError();
				}
				break;
			case ScoreKeypadClear:
				clear();
				break;
		}
		setText();

		return true;
	}
}

void ScoreInputWidget::clear(void)
{
	LOG_DEBUG("-");

	this->error = false;
	factor = Points::SingleFactor;
	value = -1;
	setText();
}

void ScoreInputWidget::setError(void)
{
	LOG_DEBUG("-");

	clear();
	this->error = true;
	Fl::add_timeout(3,(Fl_Timeout_Handler) ScoreInputWidget::resetError,this);
}

void ScoreInputWidget::resetError(ScoreInputWidget *widget)
{
	LOG_DEBUG("-");

	if (widget->error)
		widget->clear();
}

void ScoreInputWidget::setText(void)
{
	LOG_DEBUG("factor=%d  value=%d",factor,value);

	if (_config->getConfigInt(ConfigKeys::OptionsKeypadPointsFactor,1)) {
		// input method points/factor
		if (value < 0)
			text =  (error) ? "X" : "...";
		else {
			text = Points::toString(factor,value).c_str();
			error = false;
		}
	}
	else {
		// input method factor/points
		if (error)
			text =  "X";
		else {
			text = (factor == Points::SingleFactor && value < 0) ? "..." : Points::toString(factor,value).c_str();
			error = false;
		}
	}
	redraw();
}

void ScoreInputWidget::draw(void)
{
	// LOG_DEBUG("-");

	fl_push_clip(x(),y(),w(),h());

	fl_font(labelfont(),labelsize());

	fl_draw_box(FL_FLAT_BOX,x(),y(),w(),h(),error ? FL_RED : color());

	int tw = fl_width(this->text.c_str());
	fl_color(labelcolor());
	fl_draw(this->text.c_str(),x() + (w() - tw) / 2,y() + h() - (h() - labelsize() + fl_descent()) / 2);

	fl_pop_clip();
}

ScoreInputWidget::~ScoreInputWidget()
{
	LOG_DEBUG("-");
}/**/
