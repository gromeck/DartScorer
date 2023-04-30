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
#ifndef __GAMEUI_H__
#define __GAMEUI_H__

#include "common.h"
#include "GameX01Widget.h"
#include "PointsQueue.h"
#include "DartScorerGUI.h"
#include "Database.h"
#include "PlayerManagementUI.h"
#include "StatisticsWidget.h"

class GameUI : public GameGUI
{
	private:
		bool fullscreen = false;
		bool autoplay = true;

		/*
		**	the queue of points
		*/
		PointsQueue pointsQueue;
		PointsQueue pointsQueueUndone;

		/*
		**	page to return if user decides not to exit
		*/
		Fl_Widget *returnWizardPage = NULL;

		/*
		**	supported games
		*/
		GameX01Widget *gameX01Widget = NULL;

		/*
		**	statistics chart
		*/
		StatisticsWidget *StatisticsChart = NULL;

	public:
		GameUI(bool fullscreen = false,bool autoplay = false);
		~GameUI();

		bool setFullscreen(void);
		bool setFullscreen(bool fullscreen);
		bool setAutoplay(bool autoplay = true);

		void returnToLastPage(void);

		void startGame(void);
		bool replayGame(void);
		void stopGame(void);

		void autoplayPlay(void);

	private:
		std::vector<Player> playerList;
		void loadAllPlayers(void);

		static void next(Fl_Widget *widget,GameUI *ui) { ui->Wizard->next(); };
		static void back(Fl_Widget *widget,GameUI *ui) { ui->Wizard->prev(); };

		static void welcomeExitButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnWizardPage = ui->Welcome; ui->Wizard->value(ui->GameQuit); };
		static void welcomeInfoButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnWizardPage = ui->Welcome; ui->Wizard->value(ui->Info); };
		static void welcomeOptionsButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnWizardPage = ui->Welcome; ui->Wizard->value(ui->Options); };
		static void welcomeStatisticsButtonPressed(Fl_Widget *widget,GameUI *ui);

		void activateChoosePlayersNext(void);

		static void choosePlayersInButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void choosePlayersOutButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void choosePlayersUpButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void choosePlayersDownButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void choosePlayersManageButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void choosePlayersNextButtonPressed(Fl_Widget *widget,GameUI *ui);

		static void GameX01OptionsStartButtonPressed(Fl_Widget *widget,GameUI *ui);

		static void gameOnRestartButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void gameOnExitButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void gameOnCancelButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnWizardPage = ui->GameOn; ui->Wizard->value(ui->GameCancel); };
		static void gameOnOptionsButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnWizardPage = ui->GameOn; ui->Wizard->value(ui->Options); };

		static void gameCancelNoButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnToLastPage(); };
		static void gameCancelYesButtonPressed(Fl_Widget *widget,GameUI *ui);

		static void gameQuitNoButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnToLastPage(); };
		static void gameQuitYesButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->window->hide(); };

		static void gameCloseWindowPressed(Fl_Widget *widget,GameUI *ui);

		static void infoBackButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnToLastPage(); };
		static void optionsBackButtonPressed(Fl_Widget *widget,GameUI *ui);
		static void statisticsBackButtonPressed(Fl_Widget *widget,GameUI *ui) { ui->returnToLastPage(); };

		static void triggerOnGameOver(GameUI *ui);
		static bool triggerOnScoreInput(GameUI *ui,Points& points);
		static bool triggerOnScoreUndo(GameUI *ui);
		static bool triggerOnScoreRedo(GameUI *ui);

		static void playerListAllEntryClicked(Fl_Widget *widget,GameUI *ui);
		static void playerListSelectedEntryClicked(Fl_Widget *widget,GameUI *ui);

		static void optionsCallerVoiceChanged(Fl_Widget *widget,GameUI *ui);

		static void statisticsSelectorChanged(Fl_Widget *widget,GameUI *ui);

		void scoreHistoryUpdate(void);
};

#endif

/**/
