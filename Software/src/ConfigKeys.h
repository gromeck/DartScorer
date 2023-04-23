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
#ifndef __CONFIGKEYS_H__
#define __CONFIGKEYS_H__

#include "common.h"

class ConfigKeys {

	public:
		static constexpr const char *Players						= "Game.Players";
		static constexpr const char *GameGame		 				= "Game.Game";
		static constexpr const char *GameX01DoubleOut	 			= "Game.X01.DoubleOut";
		static constexpr const char *GameX01LegsToSet	 			= "Game.X01.LegsToSet";
		static constexpr const char *GameX01SetsToWin	 			= "Game.X01.SetsToWin";
		static constexpr const char *GameX01PointsToLeg				= "Game.X01.PointsToLeg";
		static constexpr const char *OptionsKeypadLayoutPhone		= "Options.KeypadLayoutPhone";
		static constexpr const char *OptionsKeypadPointsFactor		= "Options.KeypadPointsFactor";
		static constexpr const char *OptionsShowBullAs25			= "Options.ShowBullAs25";
		static constexpr const char *OptionsTableColumnTitlePlayers	= "Options.TableColumnTitle.Players";
		static constexpr const char *OptionsTableColumnTitlePoints	= "Options.TableColumnTitle.Points";
		static constexpr const char *OptionsCallerVoice				= "Options.Caller.Voice";
		static constexpr const char *OptionsCallerEachDart			= "Options.Caller.EachDart";
		static constexpr const char *StatisticsType					= "Statistics.Type";
		static constexpr const char *StatisticsPlayerA				= "Statistics.PlayerA";
		static constexpr const char *StatisticsPlayerB				= "Statistics.PlayerB";
};

#endif

/**/
