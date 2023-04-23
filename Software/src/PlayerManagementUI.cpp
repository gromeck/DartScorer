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
#include "PlayerManagementUI.h"

PlayerManagementUI::PlayerManagementUI()
{
	LOG_DEBUG("-");

	/*
	**	set the button callbacks
	*/
	List->callback((Fl_Callback *) &listEntryClicked,this);
	New->callback((Fl_Callback *) &newButtonPressed,this);
	Edit->callback((Fl_Callback *) &editButtonPressed,this);
	Delete->callback((Fl_Callback *) &deleteButtonPressed,this);
	Close->callback((Fl_Callback *) &closeButtonPressed,this);

	/*
	**	load players
	*/
	loadPlayers();
}

void PlayerManagementUI::loadPlayers(void)
{
	playerList = _database->getAllPlayers();

	List->clear();
	for (std::vector <Player>::iterator player = playerList.begin();player != playerList.end();player++) {
		List->add(player->getName().c_str(),(void *) &(*player));
		LOG_DEBUG("name=%s  player=%s",player->getName(),PTR2STR(&(*player)));
	}
	List->do_callback();
}

void PlayerManagementUI::listEntryClicked(Fl_Widget *widget,PlayerManagementUI *ui)
{
	LOG_DEBUG("value=%d",ui->List->value());

	if (ui->List->value() > 0) {
		ui->Edit->activate();
		ui->Delete->activate();
		if (Fl::event_clicks()) {
			Fl::event_clicks(0);
			ui->Edit->do_callback();
		}
	}
	else {
		ui->Edit->deactivate();
		ui->Delete->deactivate();
	}
}

void PlayerManagementUI::newButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui)
{
	LOG_DEBUG("-");

	PlayerManagementEditUI *playerManagementEditUI = new PlayerManagementEditUI();
	playerManagementEditUI->window->show();
	while (playerManagementEditUI->window->shown())
		Fl::wait();

	if (playerManagementEditUI->isWritten()) {
		ui->loadPlayers();
		// ensure that the new player is visible & selected
		for (int n = 1;n <= ui->List->size();n++)
			if (((Player *) ui->List->data(n))->getPid() == _database->getLastInsertRowId()) {
				ui->List->bottomline(n);
				ui->List->select(n);
				break;
			}
		ui->List->do_callback();
	}
	delete playerManagementEditUI;
}

void PlayerManagementUI::editButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui)
{
	LOG_DEBUG("-");

	int lineId = ui->List->value();

	if (lineId > 0) {
		LOG_DEBUG("lineId=%d  player=%s",lineId,PTR2STR(ui->List->data(lineId)));
		PlayerManagementEditUI *playerManagementEditUI = new PlayerManagementEditUI((Player *) ui->List->data(lineId));
		playerManagementEditUI->window->show();
		while (playerManagementEditUI->window->shown())
			Fl::wait();

		if (playerManagementEditUI->isWritten()) {
			ui->loadPlayers();
			ui->List->bottomline(lineId);
			ui->List->select(lineId);
			ui->List->do_callback();
		}
		delete playerManagementEditUI;
	}
}

void PlayerManagementUI::deleteButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui)
{
	LOG_DEBUG("-");

	int lineId = ui->List->value();

	if (lineId > 0 && fl_choice(_("Really delete player '%s'?"),_("Yes"),_("No"),0,ui->List->text(lineId)) == 0) {
		LOG_DEBUG("lineId=%d  player=%s",lineId,PTR2STR(ui->List->data(lineId)));
		_database->removePlayer(((Player *) ui->List->data(lineId))->getPid());
		ui->loadPlayers();
	}
}

void PlayerManagementUI::closeButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui)
{
	LOG_DEBUG("-");

	ui->window->hide();
}

PlayerManagementUI::~PlayerManagementUI()
{
	LOG_DEBUG("-");
}/**/
