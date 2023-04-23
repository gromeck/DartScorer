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
#ifndef __PLAYERMANAGEMENTEDITUI_H__
#define __PLAYERMANAGEMENTEDITUI_H__

#include "common.h"
#include "Player.h"
#include "Database.h"
#include "DartScorer.h"

class PlayerManagementEditUI : public PlayerManagementEditGUI
{
	public:
		PlayerManagementEditUI();
		PlayerManagementEditUI(Player& player);
		PlayerManagementEditUI(Player *player);
		~PlayerManagementEditUI();

		void setPlayer(Player& player);
		Player& getPlayer(void);

		bool isWritten(void) { return written; };

	private:
		bool edit = false;
		bool written = false;
		Player player;

		static void okButtonPressed(Fl_Widget *button,PlayerManagementEditUI *ui);
		static void cancelButtonPressed(Fl_Widget *button,PlayerManagementEditUI *ui);
};

#endif

/**/
