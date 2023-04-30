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
#include "DartScorer.h"

GameUI::GameUI(bool fullscreen,bool autoplay)
{
	LOG_DEBUG("fullscreen=%b  autoplay=%b",fullscreen,autoplay);

	/*
	**	set the button et al callbacks
	*/
	WelcomeExit->callback((Fl_Callback *) welcomeExitButtonPressed,this);
	WelcomeInfo->callback((Fl_Callback *) welcomeInfoButtonPressed,this);
	WelcomeOptions->callback((Fl_Callback *) welcomeOptionsButtonPressed,this);
	WelcomeStatistics->callback((Fl_Callback *) welcomeStatisticsButtonPressed,this);
	WelcomePlay->callback((Fl_Callback *) next,this);

	ChoosePlayersIn->callback((Fl_Callback *) choosePlayersInButtonPressed,this);
	ChoosePlayersOut->callback((Fl_Callback *) choosePlayersOutButtonPressed,this);
	ChoosePlayersUp->callback((Fl_Callback *) choosePlayersUpButtonPressed,this);
	ChoosePlayersDown->callback((Fl_Callback *) choosePlayersDownButtonPressed,this);
	ChoosePlayersManage->callback((Fl_Callback *) choosePlayersManageButtonPressed,this);
	ChoosePlayersNext->callback((Fl_Callback *) choosePlayersNextButtonPressed,this);
	ChoosePlayersBack->callback((Fl_Callback *) back,this);

	PlayerListAll->callback((Fl_Callback *) playerListAllEntryClicked,this);
	PlayerListSelected->callback((Fl_Callback *) playerListSelectedEntryClicked,this);

	ChooseGameBack->callback((Fl_Callback *) back,this);
	ChooseGameNext->callback((Fl_Callback *) next,this);

	GameX01OptionsBack->callback((Fl_Callback *) back,this);
	GameX01OptionsStart->callback((Fl_Callback *) GameX01OptionsStartButtonPressed,this);

	GameOnRestart->callback((Fl_Callback *) gameOnRestartButtonPressed,this);
	GameOnExit->callback((Fl_Callback *) gameOnExitButtonPressed,this);
	GameOnCancel->callback((Fl_Callback *) gameOnCancelButtonPressed,this);
	GameOnOptions->callback((Fl_Callback *) gameOnOptionsButtonPressed,this);
	ScoreInput->callbackOnScoreInput((bool (*)(void*, Points&)) triggerOnScoreInput,this);
	ScoreInput->callbackOnScoreUndo((bool (*)(void*)) triggerOnScoreUndo,this);
	ScoreInput->callbackOnScoreRedo((bool (*)(void*)) triggerOnScoreRedo,this);

	GameCancelNo->callback((Fl_Callback *) gameCancelNoButtonPressed,this);
	GameCancelYes->callback((Fl_Callback *) gameCancelYesButtonPressed,this);

	GameQuitNo->callback((Fl_Callback *) gameQuitNoButtonPressed,this);
	GameQuitYes->callback((Fl_Callback *) gameQuitYesButtonPressed,this);

	InfoBack->callback((Fl_Callback *) infoBackButtonPressed,this);
	OptionsBack->callback((Fl_Callback *) optionsBackButtonPressed,this);
	OptionsCallerVoice->callback((Fl_Callback *) optionsCallerVoiceChanged,this);
	StatisticsBack->callback((Fl_Callback *) statisticsBackButtonPressed,this);
	StatisticsType->callback((Fl_Callback *) statisticsSelectorChanged,this);
	StatisticsPlayerA->callback((Fl_Callback *) statisticsSelectorChanged,this);
	StatisticsPlayerB->callback((Fl_Callback *) statisticsSelectorChanged,this);

	this->window->callback((Fl_Callback *) gameCloseWindowPressed,this);

	/*
	**	get the available players
	*/
	loadAllPlayers();

	/*
	**	set the games
	*/
	Game->add("X01");

	/*
	**	restore the last used game
	*/
	int idx = 0;
	std::string game = _config->getConfigString(ConfigKeys::GameGame,"X01");
	if ((idx = Game->find_index(game.c_str())) < 0)
		idx = 0;
	Game->value(idx);

	/*
	**	set the X01 game options
	*/
	PointsToLeg->add("301");
	PointsToLeg->add("501");
	PointsToLeg->add("701");
	PointsToLeg->add("1001");
	LegsToSet->step(1);
	LegsToSet->bounds(2,20);
	SetsToWin->step(1);
	SetsToWin->bounds(1,10);

	/*
	**	restore the X01 game settings last used
	*/
	if (_config->getConfigInt(ConfigKeys::GameX01DoubleOut,1))
		DoubleOut->setonly();
	else
		StraightOut->setonly();
	LegsToSet->value(_config->getConfigInt(ConfigKeys::GameX01LegsToSet,3));
	SetsToWin->value(_config->getConfigInt(ConfigKeys::GameX01SetsToWin,2));
	int x01 = _config->getConfigInt(ConfigKeys::GameX01PointsToLeg,501);
	if ((idx = PointsToLeg->find_index(std::to_string(x01).c_str())) < 0)
		idx = 0;
	PointsToLeg->value(idx);

	/*
	**	info
	*/
	InfoText->value(
		(
			std::string(PACKAGE_NAME ", Version " PACKAGE_VERSION) +
			"\n" +
			"(c) 2023 by Christian Lorenz" "\n"
			"\n" +
			Poco::format(_("Released under %s"),std::string("GNU General Public License, GPL-2.0"))+
			"\n"
			"https://github.com/gromeck/DartScorer"
			"\n" +
			"\n" +
			_("This software is based on the following libraries:") +
			"\n" +
			" • " + Poco::format("PocoLib %d.%d.%d, https://pocoproject.org/",
					static_cast<int>(Poco::Environment::libraryVersion() >> 24),
					static_cast<int>((Poco::Environment::libraryVersion() >> 16) & 0xFF),
					static_cast<int>((Poco::Environment::libraryVersion() >> 8) & 0xFF)) +
			"\n" +
			" • " + Poco::format("FLTK %d.%d.%d, https://www.fltk.org/",FL_MAJOR_VERSION,FL_MINOR_VERSION,FL_PATCH_VERSION) +
			"\n" +
			" • " + Poco::format("OpenCV %d.%d.%d, https://opencv.org/",CV_MAJOR_VERSION,CV_MINOR_VERSION,CV_SUBMINOR_VERSION) +
			"\n" +
			" • " + Poco::format("PortAudio %d.%d.%d, http://www.portaudio.com/",(Pa_GetVersion() >> 16) & 0xff,(Pa_GetVersion() >> 8) & 0xff,Pa_GetVersion() & 0xff) +
			"\n"
		).c_str());

	/*
	**	options
	*/
	if (_config->getConfigInt(ConfigKeys::OptionsShowBullAs25,1))
		OptionsShowBullAs25->setonly();
	else
		OptionsShowBullAsBull->setonly();
	if (_config->getConfigInt(ConfigKeys::OptionsKeypadLayoutPhone,1))
		OptionsKeypadLayoutPhone->setonly();
	else
		OptionsKeypadLayoutPC->setonly();
	if (_config->getConfigInt(ConfigKeys::OptionsKeypadPointsFactor,1))
		OptionsKeypadPointsFactor->setonly();
	else
		OptionsKeypadFactorPoints->setonly();
	OptionsTableColumnTitlePlayers->value(_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePlayers,0));
	OptionsTableColumnTitlePoints->value(_config->getConfigInt(ConfigKeys::OptionsTableColumnTitlePoints,0));

	OptionsCallerVoice->clear();
	OptionsCallerVoice->add(_("none"));
	std::vector<std::string> voiceList = _voice->getVoices();
	for (int n = 0;n < (int) voiceList.size();n++)
		OptionsCallerVoice->add(voiceList[n].c_str());
	if ((idx = OptionsCallerVoice->find_index(_config->getConfigString(ConfigKeys::OptionsCallerVoice).c_str())) < 0)
		idx = 0;
	OptionsCallerVoice->value(idx);

	if (_config->getConfigInt(ConfigKeys::OptionsCallerEachDart,0))
		OptionsCallerEachDart->setonly();
	else
		OptionsCallerTurnTotal->setonly();

	/*
	**	statistics
	*/
	StatisticsChart = new StatisticsWidget(StatisticsArea->x(),StatisticsArea->y(),StatisticsArea->w(),StatisticsArea->h(),Statistics::TypeAverage);
	StatisticsArea->add(StatisticsChart);
	StatisticsChart->setColors(StatisticsPlayerA->labelcolor(),StatisticsPlayerB->labelcolor());

	/*
	**	set the given flags
	*/
	setFullscreen(fullscreen);
	setAutoplay(autoplay);

	/*
	**	let's start with the first page
	*/
	Wizard->value(Welcome);
}

bool GameUI::setFullscreen(void)
{
	return setFullscreen(!window->fullscreen_active());
}

bool GameUI::setFullscreen(bool fullscreen)
{
	LOG_DEBUG("fullscreen=%b",fullscreen);

	if ((this->fullscreen = fullscreen))
		window->fullscreen();
	else
		window->fullscreen_off();

	return this->fullscreen;
}

bool GameUI::setAutoplay(bool autoplay)
{
	LOG_DEBUG("autoplay=%b",autoplay);

	if ((this->autoplay = autoplay))
		GameX01OptionsStart->label(_("Autoplay"));

	return this->autoplay;
}

void GameUI::loadAllPlayers(void)
{
	playerList = _database->getAllPlayers();

	PlayerListAll->clear();
	for (std::vector <Player>::iterator player = playerList.begin();player != playerList.end();player++) {
		PlayerListAll->add(player->getName().c_str(),(void *) &(*player));
		//LOG_DEBUG("[%d] %s",player->getId(),player->getName());
	}

	PlayerListSelected->clear();
	std::string selectedPlayers = _config->getConfigString(ConfigKeys::Players);
	selectedPlayers += ",";	// ensure that we don't miss the last element
	int end;
	LOG_DEBUG("selectedPlayers=%s",selectedPlayers);
	while ((end = selectedPlayers.find(",")) >= 0) {
		int pid = atoi(selectedPlayers.substr(0,end).c_str());
		LOG_DEBUG("found pid=%d",pid);

		for (std::vector <Player>::iterator player = playerList.begin();player != playerList.end();player++) {
			if (pid == player->getPid()) {
				int alreadySelected = false;

				for (int n = 1;n <= (int) PlayerListSelected->size();n++)
					if (PlayerListSelected->data(n) == (void *) &(*player)) {
						LOG_DEBUG("pid=%d found already at line %d",pid,n);
						alreadySelected = true;
						break;
					}
				if (!alreadySelected)
					PlayerListSelected->add(player->getName().c_str(),(void *) &(*player));
				break;
			}
		}
		selectedPlayers.erase(selectedPlayers.begin(),selectedPlayers.begin() + end + 1);
	}

	PlayerListAll->do_callback();
	PlayerListSelected->do_callback();
	activateChoosePlayersNext();
}

void GameUI::returnToLastPage(void)
{
	Fl_Widget *wizardPage = (returnWizardPage) ? returnWizardPage : Welcome;
	returnWizardPage = NULL;
	Wizard->value(wizardPage);
}

void GameUI::welcomeStatisticsButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	int idx;

	ui->returnWizardPage = ui->Welcome;
	ui->Wizard->value(ui->GameStatistics);

	// set up the type selector
	ui->StatisticsType->clear();
	for (int type = Statistics::TypeMin;type <= Statistics::TypeMax;type++)
		ui->StatisticsType->add(Statistics::Type2Name(type));

	// select the last type used
	if ((idx = ui->StatisticsType->find_index(_config->getConfigString(ConfigKeys::StatisticsType).c_str())) < 0)
		idx = 0;
	ui->StatisticsType->value(idx);

	// set up the player selector
	ui->StatisticsPlayerA->clear();
	ui->StatisticsPlayerB->clear();
	ui->StatisticsPlayerB->add("-");
	for (std::vector <Player>::iterator player = ui->playerList.begin();player != ui->playerList.end();player++) {
		ui->StatisticsPlayerA->add(player->getName().c_str());
		ui->StatisticsPlayerB->add(player->getName().c_str());
		//LOG_DEBUG("[%d] %s",player->getId(),player->getName());
	}

	// select the last player used
	if ((idx = ui->StatisticsPlayerA->find_index(_config->getConfigString(ConfigKeys::StatisticsPlayerA).c_str())) < 0)
		idx = 0;
	ui->StatisticsPlayerA->value(idx);
	if ((idx = ui->StatisticsPlayerB->find_index(_config->getConfigString(ConfigKeys::StatisticsPlayerB).c_str())) < 0)
		idx = 0;
	ui->StatisticsPlayerB->value(idx);

	ui->StatisticsType->do_callback();
}

void GameUI::playerListAllEntryClicked(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("value=%d  doubleClick=%d",ui->PlayerListAll->value(),Fl::event_clicks());

	int selected = 0;
	for (int n = 1;n <= (int) ui->PlayerListAll->size();n++)
		selected += ui->PlayerListAll->selected(n);

	if (selected > 0) {
		ui->ChoosePlayersIn->activate();
	}
	else {
		ui->ChoosePlayersIn->deactivate();
	}
	if (Fl::event_clicks())
		choosePlayersInButtonPressed(widget,ui);
}

void GameUI::playerListSelectedEntryClicked(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("value=%d  doubleClick=%d",ui->PlayerListSelected->value(),Fl::event_clicks());

	int selected = 0;
	for (int n = 1;n <= (int) ui->PlayerListSelected->size();n++)
		selected += ui->PlayerListSelected->selected(n);

	if (selected > 0) {
		ui->ChoosePlayersOut->activate();
		ui->ChoosePlayersUp->activate();
		ui->ChoosePlayersDown->activate();
	}
	else {
		ui->ChoosePlayersOut->deactivate();
		ui->ChoosePlayersUp->deactivate();
		ui->ChoosePlayersDown->deactivate();
	}
	if (Fl::event_clicks())
		choosePlayersOutButtonPressed(widget,ui);
}

void GameUI::optionsCallerVoiceChanged(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("value=%d",ui->OptionsCallerVoice->value());

	_voice->setVoice(ui->OptionsCallerVoice->text());
	_voice->say(Voice::GameOn);
}

void GameUI::statisticsSelectorChanged(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("value=%d",ui->StatisticsType->value());

	int type = Statistics::Name2Type(ui->StatisticsType->text());

	if (type != Statistics::TypeUnknown) {
		ui->StatisticsChart->setType(type);
		_config->setConfigString(ConfigKeys::StatisticsType,Statistics::Type2Name(type));

		ui->StatisticsChart->clearDataA();
		if (ui->StatisticsPlayerA->text()) {
			std::string name(ui->StatisticsPlayerA->text());
			Player player = _database->getPlayer(name);

			if (player.getPid() >= 0) {
				std::vector<Statistics> list = _database->getStatistics(player.getPid(),type,100);
				ui->StatisticsChart->setDataA(list);
				_config->setConfigString(ConfigKeys::StatisticsPlayerA,name);
			}
		}
		ui->StatisticsChart->clearDataB();
		if (ui->StatisticsPlayerB->text()) {
			std::string name(ui->StatisticsPlayerB->text());
			Player player = _database->getPlayer(name);

			if (player.getPid() >= 0) {
				std::vector<Statistics> list = _database->getStatistics(player.getPid(),type,100);
				ui->StatisticsChart->setDataB(list);
				_config->setConfigString(ConfigKeys::StatisticsPlayerB,name);
			}
		}
	}
	ui->StatisticsChart->redraw();
}

void GameUI::activateChoosePlayersNext(void)
{
	if (PlayerListSelected->size() > 0)
		ChoosePlayersNext->activate();
	else
		ChoosePlayersNext->deactivate();
}

void GameUI::choosePlayersInButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	for (int n = 1;n <= (int) ui->PlayerListAll->size();n++)
		if (ui->PlayerListAll->selected(n)) {
			int alreadySelected = false;

			for (int m = 1;m <= (int) ui->PlayerListSelected->size();m++)
				if (ui->PlayerListAll->data(n) == ui->PlayerListSelected->data(m)) {
					alreadySelected = true;
					break;
				}
			if (!alreadySelected)
				ui->PlayerListSelected->add(ui->PlayerListAll->text(n),ui->PlayerListAll->data(n));
		}
	ui->PlayerListSelected->redraw();
	ui->activateChoosePlayersNext();
}

void GameUI::choosePlayersOutButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	for (int n = (int) ui->PlayerListSelected->size();n >= 1;n--)
		if (ui->PlayerListSelected->selected(n))
			ui->PlayerListSelected->remove(n);

	ui->PlayerListSelected->topline(1);
	ui->PlayerListSelected->redraw();
	ui->activateChoosePlayersNext();
}

void GameUI::choosePlayersUpButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	if (!ui->PlayerListSelected->selected(1)) {
		// don't move the selected lines up, if the first line is already a selected one
		for (int n = 1;n <= ui->PlayerListSelected->size();n++)
			if (ui->PlayerListSelected->selected(n)) {
				ui->PlayerListSelected->swap(n - 1,n);
			}
		ui->PlayerListSelected->redraw();
	}
}

void GameUI::choosePlayersDownButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	if (!ui->PlayerListSelected->selected(ui->PlayerListSelected->size())) {
		// don't move the selected lines down, if the last line is already a selected one
		for (int n = ui->PlayerListSelected->size();n >= 1;n--)
			if (ui->PlayerListSelected->selected(n)) {
				ui->PlayerListSelected->swap(n,n + 1);
			}
	}
	ui->PlayerListSelected->redraw();
}

void GameUI::choosePlayersManageButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	PlayerManagementUI *playerManagementUI = new PlayerManagementUI();
	playerManagementUI->window->show();

	while (playerManagementUI->window->shown())
		Fl::wait();

	delete playerManagementUI;

	ui->loadAllPlayers();
}

void GameUI::choosePlayersNextButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	if (!ui->PlayerListSelected->size())
		return;

	std::string selectedPlayers;

	for (int n = 1;n <= ui->PlayerListSelected->size();n++) {
		if (!selectedPlayers.empty())
			selectedPlayers += ",";
		selectedPlayers += std::to_string(((Player *) ui->PlayerListSelected->data(n))->getPid());
	}
	_config->setConfigString(ConfigKeys::Players,selectedPlayers);
	ui->Wizard->next();
}

void GameUI::GameX01OptionsStartButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	_config->setConfigString(ConfigKeys::GameGame,ui->Game->text());

	_config->setConfigInt(ConfigKeys::GameX01PointsToLeg,atoi(ui->PointsToLeg->text()));
	_config->setConfigInt(ConfigKeys::GameX01DoubleOut,ui->DoubleOut->value());
	_config->setConfigInt(ConfigKeys::GameX01LegsToSet,ui->LegsToSet->value());
	_config->setConfigInt(ConfigKeys::GameX01SetsToWin,ui->SetsToWin->value());

	ui->startGame();
}

void GameUI::gameOnRestartButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	ui->stopGame();
	ui->startGame();
}

void GameUI::gameOnExitButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	ui->stopGame();
	ui->Wizard->value(ui->ChoosePlayers);
}

void GameUI::gameCancelYesButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	ui->stopGame();
	ui->Wizard->value(ui->ChoosePlayers);
}

void GameUI::gameCloseWindowPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	if (ui->Wizard->value() == ui->GameCancel || ui->Wizard->value() == ui->GameQuit)	// ignore another window close event, if we are also on the quit page
		return;

	ui->returnWizardPage = ui->Wizard->value();
	ui->Wizard->value(ui->GameQuit);
}

void GameUI::optionsBackButtonPressed(Fl_Widget *widget,GameUI *ui)
{
	LOG_DEBUG("-");

	_config->setConfigInt(ConfigKeys::OptionsShowBullAs25,ui->OptionsShowBullAs25->value());
	_config->setConfigInt(ConfigKeys::OptionsKeypadLayoutPhone,ui->OptionsKeypadLayoutPhone->value());
	_config->setConfigInt(ConfigKeys::OptionsKeypadPointsFactor,ui->OptionsKeypadPointsFactor->value());
	_config->setConfigInt(ConfigKeys::OptionsTableColumnTitlePlayers,ui->OptionsTableColumnTitlePlayers->value());
	_config->setConfigInt(ConfigKeys::OptionsTableColumnTitlePoints,ui->OptionsTableColumnTitlePoints->value());
	_config->setConfigString(ConfigKeys::OptionsCallerVoice,ui->OptionsCallerVoice->text());
	_config->setConfigInt(ConfigKeys::OptionsCallerEachDart,ui->OptionsCallerEachDart->value());

	ui->returnToLastPage();
	if (ui->gameX01Widget) {
		// ensure to do a full redraw because the options may changed the display
		ui->gameX01Widget->forceRedraw();
		ui->ScoreHistory->setValue(ui->pointsQueue.toString(),ui->pointsQueueUndone.toStringReverse());
	}
}

void GameUI::startGame(void)
{
	/*
	**	ensure to stop the current game
	*/
	stopGame();

	LOG_DEBUG("-");

	/*
	**	clear the score queue
	*/
	pointsQueue.clear();
	pointsQueueUndone.clear();
	ScoreHistory->setValue(this->pointsQueue.toString(),this->pointsQueueUndone.toStringReverse());

	/*
	**	create the widget
	*/
	gameX01Widget = new GameX01Widget(atoi(PointsToLeg->text()),LegsToSet->value(),SetsToWin->value(),DoubleOut->value(),
		GameArea->x(),GameArea->y(),GameArea->w(),GameArea->h());

	/*
	**	get the title & mode
	*/
	GameTitle->value(gameX01Widget->getGameTitle().c_str());
	GameMode->value(gameX01Widget->getGameMode().c_str());

	/*
	**	style the widget
	*/
	gameX01Widget->box(FL_FLAT_BOX);
	gameX01Widget->color(GameOn->color());

	/*
	**	add the widget to the GameArea
	*/
	GameArea->add(gameX01Widget);

	/*
	**	disable exit & restart
	*/
	GameOnCancel->show();
	GameOnExit->hide();
	GameOnRestart->hide();

	/*
	**	configure the game
	*/
	for (int n = 1;n <= PlayerListSelected->size();n++)
		gameX01Widget->addPlayer((Player *) PlayerListSelected->data(n));
	gameX01Widget->startGame();
	gameX01Widget->callbackOnGameOver((Fl_Timeout_Handler) triggerOnGameOver,this);

	Wizard->value(GameOn);
}

bool GameUI::replayGame(void)
{
	/*
	**	it might be the case, that the players did an undo
	**	where we already thought it is the end of the game,
	**	so undoing the last dart has to reopen the right buttons
	*/
	GameOnCancel->show();
	GameOnExit->hide();
	GameOnRestart->hide();

	if (gameX01Widget) {
		/*
		**	replay the game with all the points from the queue
		*/
		gameX01Widget->startReplay();
		for (int n = 0;n < pointsQueue.length();n++) {
			Points points;

			pointsQueue.nth(n,&points);
			gameX01Widget->scoreInput(points);
		}
		gameX01Widget->stopReplay();
		return true;
	}
	return false;
}

bool GameUI::triggerOnScoreInput(GameUI *ui,Points& points)
{
	LOG_DEBUG("ui=%s  points=%s",PTR2STR(ui),points.toString());

	if (Points::validate(points) && ui->gameX01Widget && ui->gameX01Widget->scoreInput(points)) {
		/*
		**	the game accepted the scoring
		*/
		ui->pointsQueue.push(points);
		ui->pointsQueueUndone.clear();
		ui->ScoreHistory->setValue(ui->pointsQueue.toString(),ui->pointsQueueUndone.toStringReverse());
		return true;
	}
	return false;
}

bool GameUI::triggerOnScoreUndo(GameUI *ui)
{
	LOG_DEBUG("-");

	/*
	**	pop the last element -- and push it into the queue of undones
	*/
	Points undone;

	if (ui->pointsQueue.pop(&undone)) {
		ui->pointsQueueUndone.push(undone);
		ui->ScoreHistory->setValue(ui->pointsQueue.toString(),ui->pointsQueueUndone.toStringReverse());

		return ui->replayGame();
	}
	return false;
}

bool GameUI::triggerOnScoreRedo(GameUI *ui)
{
	LOG_DEBUG("-");

	/*
	**	pop the last element -- and push it into the queue of undones
	*/
	Points undone;

	if (ui->pointsQueueUndone.pop(&undone)) {
		ui->pointsQueue.push(undone);
		ui->ScoreHistory->setValue(ui->pointsQueue.toString(),ui->pointsQueueUndone.toStringReverse());

		return ui->replayGame();
	}
	return false;
}

void GameUI::triggerOnGameOver(GameUI *widget)
{
	LOG_DEBUG("-");

	widget->GameOnCancel->hide();
	widget->GameOnExit->show();
	widget->GameOnRestart->show();
}

void GameUI::stopGame(void)
{
	LOG_DEBUG("-");

	if (gameX01Widget) {
		gameX01Widget->stopGame();
		// now is the right time to store statistics
		gameX01Widget->saveStatistics();
		GameArea->remove(gameX01Widget);
		delete gameX01Widget;
		gameX01Widget = NULL;
	}
}

void GameUI::autoplayPlay(void)
{
	//LOG_DEBUG("-");

	if (autoplay && gameX01Widget && gameX01Widget->isGameOn()) {
		if (rand() % 100 < 5) {
			/*
			**	do an undo
			*/
			triggerOnScoreUndo(this);
		}
		else {
			/*
			**	play a dart
			*/
			Points points;

			points.setRandom();
			triggerOnScoreInput(this,points);
		}
	}
}

GameUI::~GameUI()
{
	LOG_DEBUG("-");
}/**/
