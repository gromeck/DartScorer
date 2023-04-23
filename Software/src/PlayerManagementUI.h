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
#ifndef __PLAYERMANAGEMENTUI_H__
#define __PLAYERMANAGEMENTUI_H__

#include "common.h"
#include "Database.h"
#include "DartScorer.h"
#include "PlayerManagementEditUI.h"

class PlayerManagementUI : public PlayerManagementGUI
{
	public:
		PlayerManagementUI();
		~PlayerManagementUI();

	private:
		std::vector<Player> playerList;

		void loadPlayers(void);

		static void newButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui);
		static void editButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui);
		static void deleteButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui);
		static void closeButtonPressed(Fl_Widget *widget,PlayerManagementUI *ui);
		static void listEntryClicked(Fl_Widget *widget,PlayerManagementUI *ui);
};

#endif

/**/
