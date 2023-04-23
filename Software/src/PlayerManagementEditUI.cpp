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
#include "PlayerManagementEditUI.h"

PlayerManagementEditUI::PlayerManagementEditUI()
{
	LOG_DEBUG("-");

	/*
	**	set the button callbacks
	*/
	OK->callback((void (*)(Fl_Widget*, void*)) &okButtonPressed,this);
	Cancel->callback((void (*)(Fl_Widget*, void*)) &cancelButtonPressed,this);

	/*
	**	set the focus
	*/
	Name->take_focus();
}

PlayerManagementEditUI::PlayerManagementEditUI(Player& player) : PlayerManagementEditUI()
{
	LOG_DEBUG("playerName=%s",player.getName());

	setPlayer(player);
}

PlayerManagementEditUI::PlayerManagementEditUI(Player *player) : PlayerManagementEditUI()
{
	LOG_DEBUG("playerName=%s",player->getName());

	setPlayer(*player);
}

void PlayerManagementEditUI::setPlayer(Player& player)
{
	LOG_DEBUG("playerName=%s",player.getName());

	edit = true;
	this->player = player;
	Name->value(player.getName().c_str());
}

Player& PlayerManagementEditUI::getPlayer(void)
{
	return this->player;
}

void PlayerManagementEditUI::okButtonPressed(Fl_Widget *button,PlayerManagementEditUI *ui)
{
	LOG_DEBUG("-");

	ui->player.setName(ui->Name->value());

	if (ui->player.isValid(!ui->edit)) {
		bool rc = false;

		if (ui->edit)
			rc = _database->modifyPlayer(ui->player);
		else
			rc = _database->addPlayer(ui->player);
		if (rc) {
			// write was successful
			ui->written = true;
			ui->window->hide();
		}
		else {
			ui->Name->color(FL_RED);
			ui->Name->redraw();
		}
	}
}

void PlayerManagementEditUI::cancelButtonPressed(Fl_Widget *button,PlayerManagementEditUI *ui)
{
	LOG_DEBUG("-");

	ui->player.clear();
	ui->window->hide();
}

PlayerManagementEditUI::~PlayerManagementEditUI()
{
	LOG_DEBUG("-");
}/**/
