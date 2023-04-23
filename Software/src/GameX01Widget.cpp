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
#include "GameX01Widget.h"
#include "DartScorer.h"

GameX01Widget::GameX01Widget(int x01,int legsToSet,int setsToWin,bool doubleOut,int x,int y,int w,int h) : Fl_Box::Fl_Box(x,y,w,h,"")
{
	LOG_DEBUG("X01=%d  legsToSet=%d  setsToWin=%d  doubleOut=%b  x=%d  y=%d  w=%d  h=%d",X01,legsToSet,setsToWin,doubleOut,x,y,w,h);

	if (x01 % 100 == 1 && x01 > 100)
		X01 = x01;
	if (legsToSet > 0)
		LegsToSet = legsToSet;
	if (setsToWin > 0)
		SetsToWin = setsToWin;
	DoubleOut = doubleOut;

	currentPlayer = 0;
	totalDarts = 0;
	totalLegs = 0;
	totalSets = 0;

	statistics.clear();

	background = new Fl_JPEG_Image(DATADIR "background.jpg");

	print();
}

std::string GameX01Widget::getGameMode(void)
{
	std::string mode;

	mode = std::to_string(X01) + ", " + (DoubleOut ? "DOUBLE" : "STRAIGHT") + " OUT,\n";
	if (SetsToWin > 1)
		mode += std::to_string(LegsToSet) + " " + _("LEGS FOR A SET") + ", "  + std::to_string(SetsToWin) + " " + _("SETS FOR THE GAME");
	else
		mode += std::to_string(LegsToSet) + " " + _("LEGS FOR THE GAME");

	return mode.c_str();
}

void GameX01Widget::addPlayer(Player *player)
{
	LOG_DEBUG("name=%s",player->getName());

	if (!gameOn) {
		/*
		**	add player name
		*/
		_playerEntry *playerEntry = new _playerEntry;
		playerEntry->player = *player;
		playerList.push_back(*playerEntry);

		if (!replay) {
			redraw();
			print();
		}
	}
}

void GameX01Widget::addPlayer(Player& player)
{
	LOG_DEBUG("name=%s",player.getName());
	return addPlayer(&player);
}

void GameX01Widget::print(void)
{
#if 1
	if (!replay) {
		/*
		**	print the table
		*/
		LOG_DEBUG("currentPlayer=%d",currentPlayer);
		LOG_DEBUG("totalDarts=%d",   totalDarts);
		LOG_DEBUG("totalLegs=%d",    totalLegs);
		LOG_DEBUG("totalSets=%d",    totalSets);

		LOG_DEBUG("Player|dartsPerGame|dartsPerSet|dartsPerLeg|dartsPerTurn|pointsLeftBeforeTurn|pointsLeft|legs|sets|Turn         |Checkout   |");
		LOG_DEBUG("------+------------+-----------+-----------+------------+--------------------+----------+----+----+-------------+-----------+");
		for (int n = 0;n < (int) playerList.size();n++) {
			checkoutProposal(n);
			LOG_DEBUG("%5d%s|%12d|%11d|%11d|%12d|%20d|%10d|%4d|%4d|%1s %-15s|%-15s|",n,
				std::string((startingPlayer == n) ? "*" : " "),
				playerList[n].dartsPerGame,
				playerList[n].dartsPerSet,
				playerList[n].dartsPerLeg,
				playerList[n].turn.getDarts(),
				playerList[n].pointsLeftBeforeTurn,
				playerList[n].pointsLeft,
				playerList[n].legs,
				playerList[n].sets,
				std::string((currentPlayer == n) ? "<" : ""),
				playerList[n].turn.toString(),
				playerList[n].proposal.toString());
		}
	}
#endif
}

void GameX01Widget::callbackOnGameOver(Fl_Timeout_Handler handler,void *data)
{
	callbackOnGameOverHandler = handler;
	callbackOnGameOverData = data;
}

void GameX01Widget::saveStatistics(void)
{
	LOG_DEBUG("-");

	for (int n = 0;n < (int) statistics.size();n++)
		_database->addStatistics(statistics[n]);
}

void GameX01Widget::startGame(void)
{
	LOG_DEBUG("-");

	if (playerList.size()) {
		startReplay();
		stopReplay();

		setMessage(_("Game on!"),10);
		caller(Voice::GameOn);
	}
}

void GameX01Widget::startReplay(void)
{
	LOG_DEBUG("-");

	replay = true;

	currentPlayer = 0;
	totalDarts = 0;
	totalLegs = 0;
	totalSets = 0;

	statistics.clear();

	for (int n = 0;n < (int) playerList.size();n++) {
		playerList[n].dartsPerGame = 0;
		playerList[n].legs = 0;
		playerList[n].sets = 0;
	}

	gameOn = true;

	startSet();
}

void GameX01Widget::stopReplay(void)
{
	LOG_DEBUG("-");

	Fl::remove_timeout((Fl_Timeout_Handler) GameX01Widget::resetMessage);
	Fl::remove_timeout((Fl_Timeout_Handler) GameX01Widget::showLastPoints);
	replay = false;

	showLastTurn = false;
	checkoutProposal();
	clearMessage();
	print();
	redraw();
}

void GameX01Widget::stopGame(void)
{
	LOG_DEBUG("-");

	gameOn = false;
}

bool GameX01Widget::isGameOn(void)
{
	//LOG_DEBUG("-");

	return gameOn;
}

void GameX01Widget::startSet(void)
{
	LOG_DEBUG("-");

	for (int n = 0;n < (int) playerList.size();n++) {
		playerList[n].dartsPerSet = 0;
	}
	startLeg();
}

void GameX01Widget::startLeg(void)
{
	LOG_DEBUG("-");

	for (int n = 0;n < (int) playerList.size();n++) {
		playerList[n].dartsPerLeg = 0;
		playerList[n].pointsLeft = X01;
		playerList[n].turn.clear();
	}
	startTurn(startingPlayer = totalLegs % playerList.size());
}

void GameX01Widget::startTurn(int n)
{
	LOG_DEBUG("-");

	currentPlayer = n % playerList.size();
	playerList[currentPlayer].pointsLeftBeforeTurn = playerList[currentPlayer].pointsLeft;
	playerList[currentPlayer].turn.clear();

	if (!replay) {
		checkoutProposal();
		print();
		redraw();
	}
}

bool GameX01Widget::scoreInput(Points& points)
{
	LOG_DEBUG("factor=%d  value=%d",points.getFactor(),points.getValue());

	if (!replay && _config->getConfigInt(ConfigKeys::OptionsCallerEachDart,0))
		_voice->say(points,true);

	if (gameOn && Points::validate(points)) {
		/*
		**	count the darts
		*/
		totalDarts++;
		playerList[currentPlayer].dartsPerSet++;
		playerList[currentPlayer].dartsPerLeg++;
		playerList[currentPlayer].dartsPerGame++;
		playerList[currentPlayer].turn.push(points);
		int points = playerList[currentPlayer].turn.getPoints();
		playerList[currentPlayer].pointsLeft = playerList[currentPlayer].pointsLeftBeforeTurn - points;
		LOG_DEBUG("currentPlayer=%d  points=%d  pointsLeft=%d  pointsLeftBeforeTurn=%d",
			currentPlayer,points,playerList[currentPlayer].pointsLeft,playerList[currentPlayer].pointsLeftBeforeTurn);

		if (playerList[currentPlayer].pointsLeft < 0 ||
			(DoubleOut && playerList[currentPlayer].pointsLeft == 0 && !playerList[currentPlayer].turn.isLastDartDouble()) ||
			(DoubleOut && playerList[currentPlayer].pointsLeft > 0 && playerList[currentPlayer].pointsLeft < Points::S1 * Points::DoubleFactor)) {
			/*
			**	busted!
			*/
			playerList[currentPlayer].pointsLeft = playerList[currentPlayer].pointsLeftBeforeTurn;
			endTurn(true);
		}
		else {
			/*
			**	player scored
			*/
			if (!replay) {
				print();
				redraw();
			}
			if (playerList[currentPlayer].pointsLeft == 0 || playerList[currentPlayer].turn.getDarts() >= Turn::DartsPerTurn)
				endTurn();
		}
		if (!replay)
			checkoutProposal();
		return true;
	}
	return false;
}

bool GameX01Widget::scoreInput(int factor,int value)
{
	Points points(factor,value);

	return scoreInput(points);
}

void GameX01Widget::endTurn(bool busted)
{
	if (!replay && !_config->getConfigInt(ConfigKeys::OptionsCallerEachDart,0)) {
		if (playerList[currentPlayer].turn.getPoints() == 0)
			_voice->say(Voice::NoScore,true);
		else
			_voice->say(playerList[currentPlayer].turn.getPoints(),true);
	}
	if (busted) {
		setMessage(Poco::format(_("%s is busted!"),playerList[currentPlayer].player.getName()),10);
		caller(Voice::Busted);
	}

	/*
	**	store some statistics
	*/
	if (playerList[currentPlayer].turn.getPoints() >= 100)
		statistics.push_back(Statistics(playerList[currentPlayer].player.getPid(),
				Statistics::TypeHighScores,
				playerList[currentPlayer].turn.getPoints()));

	if (playerList[currentPlayer].pointsLeft == 0)
		playerWonLeg();
	else {
		showLastTurn = true;
		Fl::add_timeout(5,(Fl_Timeout_Handler) GameX01Widget::showLastPoints,this);
		nextPlayer();
	}
}

void GameX01Widget::nextPlayer(void)
{
	startTurn(currentPlayer + 1);
}

void GameX01Widget::playerWonLeg(void)
{
	playerList[currentPlayer].legs++;
	totalLegs++;
	setMessage(Poco::format(_("%s won the leg with %d darts!"),playerList[currentPlayer].player.getName(),playerList[currentPlayer].dartsPerLeg),10);
	caller(Voice::WonLeg);

#if 1
	LOG_DEBUG("Player|dartsPerLeg|pointsInLeg|average|");
	LOG_DEBUG("------+-----------+-----------+-------+");
	for (int n = 0;n < (int) playerList.size();n++) {
		LOG_DEBUG("%6d|%11d|%11d|%7.2f|",n,
			playerList[n].dartsPerLeg,
			X01 - playerList[n].pointsLeft,
			(playerList[n].dartsPerLeg > 0) ? ((double) X01 - playerList[n].pointsLeft) / playerList[n].dartsPerLeg * 3 : 0);
	}
#endif

	/*
	**	store some statistics
	*/
	for (int n = 0;n < (int) playerList.size();n++)
		if (playerList[n].dartsPerLeg)
			statistics.push_back(Statistics(playerList[n].player.getPid(),
					Statistics::TypeAverage,
					(double) (X01 - playerList[n].pointsLeft) / playerList[n].dartsPerLeg * 3.0));

	statistics.push_back(Statistics(
			playerList[currentPlayer].player.getPid(),
			Statistics::TypeDartsToLeg,
			playerList[currentPlayer].dartsPerLeg));

	if (playerList[currentPlayer].turn.getPoints() >= 100)
		statistics.push_back(Statistics(
				playerList[currentPlayer].player.getPid(),
				Statistics::TypeHighFinish,
				playerList[currentPlayer].turn.getPoints()));

	if (playerList[currentPlayer].legs >= LegsToSet) {
		playerWonSet();
		return;
	}
	else
		startLeg();

	if (!replay) {
		print();
		redraw();
	}
}

void GameX01Widget::playerWonSet(void)
{
	playerList[currentPlayer].sets++;
	totalSets++;

	/*
	**	NOTE: in case were only one set is played, the set
	**	column is not displayed. This means, that the leg
	**	result should not be cleared in that case
	*/
	if (SetsToWin > 1) {
		setMessage(Poco::format(_("%s won the set!"),playerList[currentPlayer].player.getName()),10);
		caller(Voice::WonSet);
		for (int n = 0;n < (int) playerList.size();n++)
			playerList[n].legs = 0;
	}

	if (playerList[currentPlayer].sets >= SetsToWin) {
		playerWonGame();
		return;
	}
	else
		startSet();

	if (!replay) {
		print();
		redraw();
	}
}

void GameX01Widget::playerWonGame(void)
{
	LOG_DEBUG("player %d won the game",currentPlayer);

	setMessage(Poco::format(_("%s won the game!"),playerList[currentPlayer].player.getName()),0);
	caller(Voice::WonGame);
	gameOn = false;

	// trigger the outer GUI
	if (callbackOnGameOverHandler)
		(*callbackOnGameOverHandler)(callbackOnGameOverData);

	if (!replay) {
		print();
		redraw();
	}
}

/*
**	search for a short checkout combination
**
**	the algorythm does a full search as long as the checkout is still possible
**
**	if no checkout can be found, false is returned
*/
void GameX01Widget::checkoutProposalInternal(Turn *probe,int pointsLeft,Turn *proposal)
{
//	LOG_DEBUG("pointsLeft=%d  dartsLeft=%d  depth=%d",pointsLeft,dartsLeft,depth);

	if (probe->getDarts() >= Turn::DartsPerTurn || pointsLeft <= 0 || pointsLeft > (Turn::DartsPerTurn - probe->getDarts()) * Points::T20)
		return;

	for (int factor = Points::SingleFactor;factor <= Points::TrippleFactor;factor++) {
		for (int value = (factor == Points::TrippleFactor) ? Points::S20 : Points::Bull;value >= Points::S1;value--) {
			// special handling for the step from Bull to 20
			if (value == Points::Bull - 1)
				value = Points::S20;

			if (!probe->push(factor,value))
				assert(0);

#if 0
			LOG_DEBUG("pointsLeft=%d  darts=%d  probing %s (%d)  %d-%d-%d",
					pointsLeft,turn->getDarts(),turn->toString(),turn->getPoints(),
					turn->getPoints(0),turn->getPoints(1),turn->getPoints(2));
#endif

			if (probe->getPointsLastDart() == pointsLeft && (!DoubleOut || factor == Points::DoubleFactor)) {
				/*
				**	checkout found
				*/
				if (proposal->getDarts() == 0 || probe->getDarts() < proposal->getDarts()) {
					/*
					**	this checkout is shorter than the one we had so far
					*/
					*proposal = *probe;
				}
			}
			else if (probe->getPointsLastDart() < pointsLeft) {
				/*
				**	checkout still possible, try with the next dart
				*/
				checkoutProposalInternal(probe,pointsLeft - probe->getPointsLastDart(),proposal);
			}

			if (!probe->pop())
				assert(0);
		}
	}
}

bool GameX01Widget::checkoutProposal(Turn *turn,int pointsLeft,Turn *proposal)
{
	Turn probe = *turn;
	proposal->clear();
	checkoutProposalInternal(&probe,pointsLeft,proposal);
	return proposal->getDarts() ? true : false;
}

bool GameX01Widget::checkoutProposal(int playerNum)
{
	Turn turn = playerList[playerNum].turn;

	return checkoutProposal(&turn,playerList[playerNum].pointsLeft,&playerList[playerNum].proposal);
}

bool GameX01Widget::checkoutProposal(void)
{
	return checkoutProposal(currentPlayer);
}

int GameX01Widget::computeHeight(void)
{
	return MessageHeight() + SeparatorHeight() + TableHeight() + TableShadowOffsetY();
}

void GameX01Widget::clearMessage(void)
{
	message = "";
	Fl::remove_timeout((Fl_Timeout_Handler) GameX01Widget::resetMessage);
	if (!replay) {
		redraw();
	}
}

void GameX01Widget::resetMessage(GameX01Widget *widget)
{
	if (widget)
		widget->clearMessage();
}

void GameX01Widget::setMessage(std::string message,int timeout)
{
	clearMessage();
	this->message = message;
	if (timeout > 0)
		Fl::add_timeout(timeout,(Fl_Timeout_Handler) GameX01Widget::resetMessage,this);
	if (!replay) {
		redraw();
	}
}

void GameX01Widget::caller(const char *text)
{
	if (!replay)
		_voice->say(text);
}

void GameX01Widget::showLastPoints(GameX01Widget *widget)
{
	widget->showLastTurn = false;
	widget->redraw();
}

int GameX01Widget::computeWidth(void)
{
	/*
	**	find the widest digits
	*/
	fl_font(TableCellPointsFontFace,TableCellPointsFontSize());
	char buffer[2] = { 0, 0 };
	char *number = buffer;
	int widestNumberWidth = 0;
	for (*number = '0';*number <= '9';(*number)++)
		if (fl_width(number) > widestNumberWidth)
			widestNumberWidth = fl_width(number);

	/*
	**	find the widest factor/value
	*/
	int widestFactor = Points::DoubleFactor;
	int widestValue = Points::Bull;
	fl_font(TableCellTurnFontFace,TableCellTurnFontSize());
	for (int factor = Points::SingleFactor;factor <= Points::TrippleFactor;factor++)
		for (int value = Points::S1;value <= Points::S20;value++)
			if (widestFactor < 0 || fl_width(Points::toString(factor,value).c_str()) > fl_width(Points::toString(widestFactor,widestValue).c_str())) {
				widestFactor = factor;
				widestValue = value;
			}
//	LOG_DEBUG("widestFactor=%d  widestValue=%d",widestFactor,widestValue);
	Turn widestTurn;
	widestTurn.push(widestFactor,widestValue);
	widestTurn.push(widestFactor,widestValue);
	widestTurn.push(widestFactor,widestValue);

	/*
	**	get the widest proposal and turn
	*/
	fl_font(TableCellProposalFontFace,TableCellProposalFontSize());
	WidestProposalWidth = fl_width(widestTurn.toString().c_str());
	WidestProposalWidth += 2 * TablePadding();

	fl_font(TableCellTurnFontFace,TableCellTurnFontSize());
	WidestTurnWidth = fl_width(widestTurn.toString().c_str());
	WidestTurnWidth += 2 * TablePadding();

	/*
	**	find the widest player title or name
	*/
	fl_font(TableHeadlineFontFace,TableHeadlineFontSize());
	WidestPlayerWidth = (_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePlayers,0)) ? fl_width(TableCellPlayerTitle) : 0;
	fl_font(TableCellPlayerFontFace,TableCellPlayerFontSize());
	for (int n = 0;n < (int) playerList.size();n++)
		WidestPlayerWidth = DartScorer::max(WidestPlayerWidth,fl_width(playerList[n].player.getName().c_str()));
	WidestPlayerWidth += 3 * TablePadding() + TableCellPlayerMarkerSize();

	/*
	**	find the widest sets title or number
	*/
	if (SetsToWin > 1) {
		fl_font(TableHeadlineFontFace,TableHeadlineFontSize());
		WidestSetsWidth = fl_width(TableCellSetsTitle);
		fl_font(TableCellSetsFontFace,TableCellSetsFontSize());
		WidestSetsWidth = DartScorer::max(WidestSetsWidth,DartScorer::lengthOfNumber(SetsToWin) * widestNumberWidth);
		WidestSetsWidth += 2 * TablePadding();
	}
	else
		WidestSetsWidth = 0;

	/*
	**	find the widest legs title or number
	*/
	fl_font(TableHeadlineFontFace,TableHeadlineFontSize());
	WidestLegsWidth = fl_width(TableCellLegsTitle);
	fl_font(TableCellLegsFontFace,TableCellLegsFontSize());
	WidestLegsWidth = DartScorer::max(WidestLegsWidth,DartScorer::lengthOfNumber(LegsToSet) * widestNumberWidth);
	WidestLegsWidth += 2 * TablePadding();

	/*
	**	find the widest points title or number
	*/
	fl_font(TableHeadlineFontFace,TableHeadlineFontSize());
	WidestPointsWidth = (_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePoints,0)) ? fl_width(TableCellPointsTitle) : 0;
	fl_font(TableCellPointsFontFace,TableCellPointsFontSize());
	WidestPointsWidth = DartScorer::max(WidestPointsWidth,DartScorer::lengthOfNumber(X01) * widestNumberWidth);
	WidestPointsWidth += 2 * TablePadding();

	/*
	**	put everything together
	*/
	return WidestProposalWidth
			+ WidestPlayerWidth
			+ WidestSetsWidth
			+ WidestLegsWidth
			+ WidestPointsWidth
			+ WidestTurnWidth;
}

void GameX01Widget::forceRedraw(void)
{
	lastW = -1;
	lastH = -1;
	draw();
}

void GameX01Widget::draw(void)
{
	LOG_DEBUG("-");

	char buffer[100];

	fl_push_clip(x(),y(),w(),h());

	if (lastW != w() || lastH != h()) {
		/*
		**	the canvas size has changed, so compute the optimal base font size
		**	by probing if the display fits into the canvas
		*/
		int iterations = 0;
		int low = BaseFontSizeMinimum;
		int high = BaseFontSizeMaximum;
		while (high - low > 1 && iterations < 20) {
			BaseFontSize = (low + high) / 2;
			iterations++;
			if ((DisplayWidth = computeWidth()) < w() && (DisplayHeight = computeHeight()) < h())
				low = BaseFontSize; // display fits
			else
				high = BaseFontSize - 1; // display doesn't fit
		}
		BaseFontSize = low;
		LOG_DEBUG("BaseFontSize=%d found in %d iterations",BaseFontSize,iterations);

		lastW = w();
		lastH = h();
		LOG_DEBUG("DisplayWidth=%d  DisplayHeight=%d",DisplayWidth,DisplayHeight);
		if (background) {
			backgroundScaled = background->copy(w(),h());
			LOG_DEBUG("backgroundW=%d  backgrondH=%d",background->w(),background->h());
		}
		if (backgroundScaled) {
			LOG_DEBUG("backgroundScaledW=%d  backgrondScaledH=%d",backgroundScaled->w(),backgroundScaled->h());
		}
	}

	/*
	**	draw the background
	*/
	fl_draw_box(FL_FLAT_BOX,x(),y(),w(),h(),color());
	if (backgroundScaled)
		backgroundScaled->draw(x(),y());

	/*
	**	position the message & table
	*/
	int messageX = x() + (w() - DisplayWidth) / 2;
	int messageY = y() + (h() - DisplayHeight) / 2;
	int tableX = messageX;
	int tableY = messageY + MessageHeight() + SeparatorHeight();

	/*
	**	display the message
	*/
#if 0
	fl_draw_box(FL_FLAT_BOX,
			messageX,
			messageY,
			DisplayWidth,
			MessageHeight(),
			FL_YELLOW);
#endif
	fl_font(MessageFontFace,
			MessageFontSize());
	fl_push_clip(messageX,
			messageY,
			DisplayWidth,
			MessageHeight());
	fl_color(MessageFontColor);
	fl_draw(message.c_str(),
			messageX + (DisplayWidth - fl_width(message.c_str())) / 2,
			messageY + MessageHeight() / 2 + MessageFontSize() / 2 - fl_descent() / 2);
	fl_pop_clip();

	/*
	**	draw the table shadow
	*/
#if 0
	fl_draw_box(FL_FLAT_BOX,
			tableX,
			tableY,
			DisplayWidth,
			TableHeight(),
			FL_MAGENTA);
#endif
	fl_draw_box(FL_FLAT_BOX,
			tableX + TableCellProposalWidth() + TableShadowOffsetX(),
			tableY + TableShadowOffsetY(),
			TableCellPlayerWidth() + TableCellSetsWidth() + TableCellLegsWidth() + TableCellPointsWidth() + 3 * TableSpacing,
			TableHeight(),
			TableShadowColor);

	/*
	**	draw the table
	*/
	for (int n = 0;n <= (int) playerList.size();n++) {
		int x = tableX;
		int y = tableY + n * (TableRowHeight() + TableSpacing);
		int p = n - 1;
		int previousPlayer = (currentPlayer - 1 + playerList.size()) % playerList.size();

		/*
		**	proposal
		*/
		if (p == currentPlayer) {
			if (playerList[p].proposal.getDarts()) {
				y += (TableRowHeight() - TableCellProposalHeight()) / 2;
				sprintf(buffer,"%s",playerList[p].proposal.toString(&playerList[p].turn).c_str());
				fl_font(TableCellProposalFontFace,
						TableCellProposalFontSize());
				fl_draw_box(FL_FLAT_BOX,
						x + TableCellProposalWidth() - fl_width(buffer) -  2 * TablePadding(),
						y,
						fl_width(buffer) + 2 * TablePadding(),
						TableCellProposalHeight(),
						TableCellProposalColor);
				fl_push_clip(x,
						y,
						TableCellProposalWidth(),
						TableCellProposalHeight());
				fl_color(TableCellProposalFontColor);
				fl_draw(buffer,
						x + TableCellProposalWidth() - fl_width(buffer) - TablePadding(),
						y + TableCellProposalHeight() / 2 + TableCellProposalFontSize() / 2 - fl_descent() / 2);
				fl_pop_clip();
				y = tableY + n * (TableRowHeight() + TableSpacing);
			}
		}
		x += TableCellProposalWidth() + TableSpacing;

		/*
		**	player name & start marker
		*/
		fl_font((!n) ? TableHeadlineFontFace : TableCellPlayerFontFace,
				(!n) ? TableHeadlineFontSize() : TableCellPlayerFontSize());
		fl_draw_box(FL_FLAT_BOX,
				x,
				y,
				TableCellPlayerWidth(),
				TableRowHeight(),
				(!n)
					? TableHeadlineColor
					: (p == currentPlayer)
						? TableCellCurrentColor
						: TableCellPlayerColor);
		fl_push_clip(x,
			y,
			TableCellPlayerWidth(),
			TableRowHeight());
		fl_color((!n)
				? TableHeadlineFontColor
				: TableCellPlayerFontColor);
		fl_draw((!n)
					? ((_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePlayers,0))
						? TableCellPlayerTitle
						: "")
					: playerList[p].player.getName().c_str(),
				x + TablePadding(),
				y + TableRowHeight() / 2 + TableCellPlayerFontSize() / 2 - fl_descent() / 2);
		if (p == startingPlayer) {
			fl_draw_symbol(TableCellPlayerMarker,
					x  + TableCellPlayerWidth() - TableCellPlayerMarkerSize() - TablePadding(),
					y + TableRowHeight() / 2 - TableCellPlayerMarkerSize() / 2,
					TableCellPlayerMarkerSize(),
					TableCellPlayerMarkerSize(),
					TableCellPlayerMarkerColor);
		}
		fl_pop_clip();
		x += TableCellPlayerWidth() + TableSpacing;

		/*
		**	sets
		*/
		if (SetsToWin > 1) {
			if (!n)
				sprintf(buffer,"%s",TableCellSetsTitle);
			else
				sprintf(buffer,"%d",playerList[p].sets);
			fl_font((!n)
						? TableHeadlineFontFace
						: TableCellSetsFontFace,
					(!n)
						? TableHeadlineFontSize()
						: TableCellSetsFontSize());
			fl_draw_box(FL_FLAT_BOX,
					x,
					y,
					TableCellSetsWidth(),
					TableRowHeight(),
					(!n)
						? TableHeadlineColor
						: (p == currentPlayer)
							? TableCellCurrentColor
							: TableCellSetsColor);
			fl_push_clip(x,
					y,
					TableCellSetsWidth(),
					TableRowHeight());
			fl_color((!n)
					? TableHeadlineFontColor
					: TableCellSetsFontColor);
			fl_draw(buffer,
					x + TableCellSetsWidth() - fl_width(buffer) - TablePadding(),
					y + TableRowHeight() / 2 + TableCellPlayerFontSize() / 2 - fl_descent() / 2);
			fl_pop_clip();
			x += TableCellSetsWidth() + TableSpacing;
		}

		/*
		**	legs
		*/
		if (!n)
			sprintf(buffer,"%s",TableCellLegsTitle);
		else
			sprintf(buffer,"%d",playerList[p].legs);
		fl_font((!n)
				? TableHeadlineFontFace
				: TableCellLegsFontFace,
				(!n)
					? TableHeadlineFontSize()
					: TableCellLegsFontSize());
		fl_draw_box(FL_FLAT_BOX,
				x,
				y,
				TableCellLegsWidth(),
				TableRowHeight(),
				(!n)
					? TableHeadlineColor
					: (p == currentPlayer)
						? TableCellCurrentColor
						: TableCellLegsColor);
		fl_push_clip(x,
				y,
				TableCellLegsWidth(),
				TableRowHeight());
		fl_color((!n)
					? TableHeadlineFontColor
					: TableCellLegsFontColor);
		fl_draw(buffer,
				x + TableCellLegsWidth() - fl_width(buffer) - TablePadding(),
				y + TableRowHeight() / 2 + TableCellPlayerFontSize() / 2 - fl_descent() / 2);
		fl_pop_clip();
		x += TableCellLegsWidth() + TableSpacing;

		/*
		**	points
		*/
		if (!n)
			sprintf(buffer,"%s",(_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePoints,0)) ? TableCellPointsTitle : "");
		else
			sprintf(buffer,"%d",playerList[p].pointsLeft);
		fl_font((!n)
					? TableHeadlineFontFace
					: TableCellPointsFontFace,
				(!n)
					? TableHeadlineFontSize()
					: TableCellPointsFontSize());
		fl_draw_box(FL_FLAT_BOX,
				x,
				y,
				TableCellPointsWidth(),
				TableRowHeight(),
				(!n)
					? TableHeadlineColor
					: (p == currentPlayer)
						? TableCellCurrentColor
						: TableCellPointsColor);
		fl_push_clip(x,
				y,
				TableCellPointsWidth(),
				TableRowHeight());
		fl_color((!n)
				? TableHeadlineFontColor
				: TableCellPointsFontColor);
		fl_draw(buffer,
				x + TableCellPointsWidth() - fl_width(buffer) - TablePadding(),
				y + TableRowHeight() / 2 + TableCellPlayerFontSize() / 2 - fl_descent() / 2);
		fl_pop_clip();
		x += TableCellPointsWidth() + TableSpacing;

		/*
		**	turn
		*/
		if (p == currentPlayer) {
			y += (TableRowHeight() - TableCellTurnHeight()) / 2;
			fl_push_clip(x,y,TableCellTurnWidth(),TableCellTurnHeight());

			if (playerList[p].turn.getDarts()) {
				sprintf(buffer,"%s",playerList[p].turn.toString().c_str());
				fl_font(TableCellTurnFontFace,TableCellTurnFontSize());
				fl_draw_box(FL_FLAT_BOX,x,y,fl_width(buffer) + 2 * TablePadding(),TableCellTurnHeight(),TableCellTurnColor);
				fl_color(TableCellTurnFontColor);
				fl_draw(buffer,x + TablePadding(),y + TableCellTurnHeight() / 2 + TableCellTurnFontSize() / 2 - fl_descent() / 2);
			}
			else {
				fl_draw_box(FL_FLAT_BOX,x,y,TableCellTurnMarkerSize() + 2 * TablePadding(),TableCellTurnHeight(),TableCellTurnColor);
				fl_draw_symbol(TableCellTurnMarker,
						x + TablePadding(),
						y + TableCellTurnHeight() / 2 - TableCellTurnMarkerSize() / 2,
						TableCellTurnMarkerSize(),
						TableCellTurnMarkerSize(),
						TableCellTurnMarkerColor);
			}
			fl_pop_clip();
		}
		if (p == previousPlayer && playerList[p].turn.getDarts()) {
			y += (TableRowHeight() - TableCellTurnHeight()) / 2;

			if (showLastTurn)
				sprintf(buffer,"%s",playerList[p].turn.toString().c_str());
			else
				sprintf(buffer,"%d",playerList[p].turn.getPoints());

			fl_font(TableCellTurnFontFace,TableCellTurnFontSize());
			fl_draw_box(FL_FLAT_BOX,x,y,fl_width(buffer) + 2 * TablePadding(),TableCellTurnHeight(),TableCellTurnPrevColor);
			fl_push_clip(x,y,TableCellTurnWidth(),TableCellTurnHeight());
			fl_color(TableCellTurnPrevFontColor);
			fl_draw(buffer,x + TablePadding(),y + TableCellTurnHeight() / 2 + TableCellTurnFontSize() / 2 - fl_descent() / 2);
			fl_pop_clip();
		}
		x += TableCellTurnWidth() + TableSpacing;
	}

	fl_pop_clip();
}

GameX01Widget::~GameX01Widget()
{
	LOG_DEBUG("-");

	if (background)
		delete background;
	if (backgroundScaled)
		delete backgroundScaled;
	Fl::remove_timeout((Fl_Timeout_Handler) GameX01Widget::resetMessage);
	Fl::remove_timeout((Fl_Timeout_Handler) GameX01Widget::showLastPoints);
}/**/
