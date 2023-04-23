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
#ifndef __GAMEX01WIDGET_H__
#define __GAMEX01WIDGET_H__ 1

#include "common.h"
#include "Config.h"
#include "ConfigKeys.h"
#include "Turn.h"
#include "Player.h"
#include "Statistics.h"
#include "Voice.h"

class GameX01Widget : public Fl_Box
{
	private:
		/*
		**	game characteristics
		*/
		int X01 = 501;
		int LegsToSet = 3;
		int SetsToWin = 5;
		bool DoubleOut = true;

		/*
		**	game control
		*/
		bool gameOn = false;
		bool replay = false;
		int currentPlayer = 0;
		int startingPlayer = 0;

		int totalDarts = 0;
		int totalLegs = 0;
		int totalSets = 0;

		struct _playerEntry {
			Player player;
			int dartsPerGame;
			int dartsPerSet;
			int dartsPerLeg;
			int pointsLeftBeforeTurn;
			int pointsLeft;
			int legs;
			int sets;
			Turn turn;
			Turn proposal;
		};
		std::vector<_playerEntry> playerList;

		/*
		**	UI controls
		*/
		bool fullscreen = false;
		void print(void);
		Fl_Timeout_Handler callbackOnGameOverHandler = NULL;
		void *callbackOnGameOverData = NULL;

		/*
		**	control the game procedure
		*/
		void startSet(void);
		void startLeg(void);
		void startTurn(int player);
		void endTurn(bool busted = false);
		void nextPlayer(void);
		void playerWonLeg(void);
		void playerWonSet(void);
		void playerWonGame(void);

		/*
		**	info/message handling
		*/
		bool showLastTurn = false;
		static void showLastPoints(GameX01Widget *widget);
		std::string message;
		void clearMessage(void);
		static void resetMessage(GameX01Widget *widget);
		void setMessage(std::string message,int timeout);
		void caller(const char *text);

		/*
		**	internals to compute a short checkout combination
		*/
		void checkoutProposalInternal(Turn *probe,int pointsLeft,Turn *proposal);

		/*
		**	statistics will be collected during the game
		**	and written into the database at the end
		*/
		std::vector<Statistics> statistics;

		/*
		**	background image
		*/
		Fl_Image *background = NULL;
		Fl_Image *backgroundScaled = NULL;

		/*
		**	the basefont scales up and down to best fit the display
		*/
		const int BaseFontSizeMinimum = 20;
		const int BaseFontSizeMaximum = 120;
		int BaseFontSize = BaseFontSizeMinimum;

		/*
		**	dimensions of the message area
		*/
		int MessageFontSize() { return BaseFontSize * 1.25; };
		int MessageHeight() { return MessageFontSize() * 1.5; };
		int MessageFontFace = FL_HELVETICA_BOLD;
		int MessageFontColor = FL_WHITE;

		/*
		**	dimensions of the seperator area
		*/
		int SeparatorHeight() { return BaseFontSize * 1.50; };

		/*
		**	dimensions of the table content
		*/
		int TableHeight() { return (playerList.size() + 1) * (TableSpacing + TableRowHeight()) - TableSpacing; };
		int TableRowHeight() { return BaseFontSize * 1.20; };

		int TableSpacing = 1;
		int TablePadding() { return TableRowHeight() * 0.30; };

		int TableShadowOffsetX() { return TableRowHeight() * 0.50; };
		int TableShadowOffsetY() { return TableRowHeight() * 0.33; };
		int TableShadowColor = FL_DARK_BLUE;

		int TableHeadlineColor = FL_BLACK;
		int TableHeadlineFontSize() { return BaseFontSize; };
		int TableHeadlineFontFace = FL_HELVETICA;
		int TableHeadlineFontColor = FL_WHITE;

		int TableCellColor = FL_WHITE;
		int TableCellFontSize() { return BaseFontSize; };
		int TableCellFontFace = FL_HELVETICA_BOLD;
		int TableCellFontColor = FL_DARK_BLUE;
		int TableCellCurrentColor = 237;
		int TableCellCurrentFontColor = FL_DARK_BLUE;

		int TableCellProposalWidth() { return WidestProposalWidth; };
		int TableCellProposalHeight() { return TableCellProposalFontSize() * 1.10; };
		int TableCellProposalColor = FL_DARK_GREEN;
		int TableCellProposalFontSize() { return BaseFontSize * 0.90; };
		int TableCellProposalFontFace = TableCellFontFace;
		int TableCellProposalFontColor = FL_WHITE;

		int TableCellPlayerWidth() { return WidestPlayerWidth; };
		int TableCellPlayerColor = TableCellColor;
		int TableCellPlayerFontSize() { return TableCellFontSize(); };
		int TableCellPlayerFontFace = TableCellFontFace;
		int TableCellPlayerFontColor = TableCellFontColor;
		int TableCellPlayerMarkerColor = FL_DARK_RED;
		int TableCellPlayerMarkerSize() { return TableCellPlayerFontSize() * 0.60; };
		const char *TableCellPlayerMarker = "@square";
		const char *TableCellPlayerTitle = _("PLAYER");

		int TableCellSetsWidth() { return WidestSetsWidth; };
		int TableCellSetsColor = TableCellColor;
		int TableCellSetsFontSize() { return TableCellFontSize(); };
		int TableCellSetsFontFace = TableCellFontFace;
		int TableCellSetsFontColor = TableCellFontColor;
		const char *TableCellSetsTitle = _("SETS");

		int TableCellLegsWidth() { return WidestLegsWidth; };
		int TableCellLegsColor = TableCellColor;
		int TableCellLegsFontSize() { return TableCellFontSize(); };
		int TableCellLegsFontFace = TableCellFontFace;
		int TableCellLegsFontColor = TableCellFontColor;
		const char *TableCellLegsTitle = _("LEGS");

		int TableCellPointsWidth() { return WidestPointsWidth; };
		int TableCellPointsColor = TableCellColor;
		int TableCellPointsFontSize() { return TableCellFontSize(); };
		int TableCellPointsFontFace = TableCellFontFace;
		int TableCellPointsFontColor = TableCellFontColor;
		const char *TableCellPointsTitle = _("POINTS");

		int TableCellTurnWidth() { return WidestTurnWidth; };
		int TableCellTurnHeight() { return TableCellTurnFontSize() * 1.10; };
		int TableCellTurnColor = FL_DARK1;
		int TableCellTurnFontSize() { return BaseFontSize * 0.80; };
		int TableCellTurnFontFace = TableCellFontFace;
		int TableCellTurnFontColor = TableCellFontColor;
		int TableCellTurnPrevColor = FL_DARK3;
		int TableCellTurnPrevFontColor = FL_DARK1;
		int TableCellTurnMarkerSize() { return TableCellTurnHeight() * 0.90; };
		int TableCellTurnMarkerColor = FL_DARK_RED;
		const char *TableCellTurnMarker = "@<-";

		/*
		**	resulting dimensions of the display
		*/
		int WidestProposalWidth = 0;
		int WidestPlayerWidth = 0;
		int WidestSetsWidth = 0;
		int WidestLegsWidth = 0;
		int WidestPointsWidth = 0;
		int WidestTurnWidth = 0;

		/*
		**	last window dimensions -- if changed, we have to recompute the base font size
		*/
		int lastW = -1;
		int lastH = -1;

		/*
		**	overall dimensions of the display
		*/
		int DisplayWidth = 0;
		int DisplayHeight = 0;

	public:
		GameX01Widget(int x01,int legsToSet,int setsToWin,bool doubleOut,int x,int y,int w,int h);
		~GameX01Widget();

		/*
		**	UI controls
		*/
		void callbackOnGameOver(Fl_Timeout_Handler cb,void *p);

		/*
		**	game controls
		*/
		std::string getGameTitle(void) { return "X01"; };
		std::string getGameMode(void);
		void addPlayer(Player *player);
		void addPlayer(Player& player);
		void startGame(void);
		bool scoreInput(Points& points);
		bool scoreInput(int factor,int value);
		void stopGame(void);
		bool isGameOn(void);
		void saveStatistics(void);

		/*
		**	control the replay of the game
		**	(used for undo)
		*/
		void startReplay(void);
		void stopReplay(void);

		/*
		**	compute checkout proposal for the current player
		**	in the current turn
		*/
		bool checkoutProposal(void);

		/*
		**	compute checkout proposal for the given player
		**	with empty turn
		*/
		bool checkoutProposal(int player);

		/*
		**	compute a short checkout combination with given
		**	turn and points left
		*/
		bool checkoutProposal(Turn *turn,int pointsLeft,Turn *proposal);

		/*
		**	compute the dimension of the table
		*/
		int computeHeight(void);
		int computeWidth(void);

		void forceRedraw(void);
		void draw(void);
};

#endif

/**/
