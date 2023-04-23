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
#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "common.h"
#include "Config.h"
#include "Player.h"
#include "Statistics.h"

class Database {
	private:
		int lastInsertRowId = -1;

		int players = 0;
		Poco::Data::Session *session;
		const char *DbBackend = "SQLite";
		const char *DbFile = "database.sqlite3";

		static constexpr const char *DbTablePlayerName = "Player";
		static constexpr const char *DbTablePlayerDescription = " \
				pid INTEGER PRIMARY KEY, \
				name VARCHAR(50) NOT NULL UNIQUE \
				";
		static constexpr const char *DbTableStatisticsName = "Statistics";
		static constexpr const char *DbTableStatisticsDescription = " \
				pid INTEGER KEY, \
				type INTEGER KEY, \
				value DOUBLE NOT NULL, \
				timestamp TEXT NOT NULL \
				";

		struct _DbTable {
			const char *DbTableName;
			const char *DbTableDescription;
		} _DbTables[3] = {
			{ DbTablePlayerName,			DbTablePlayerDescription			},
			{ DbTableStatisticsName,		DbTableStatisticsDescription		},
			{ NULL, NULL },
		};

	public:
		Database();
		~Database();

		int getLastInsertRowId(void) { return lastInsertRowId; };

		int getPlayers(void);
		std::vector<Player> getAllPlayers(void);

		Player getPlayer(std::string& name);

		bool addPlayer(Player& player);
		bool modifyPlayer(Player& player);
		bool removePlayer(int pid);
		bool removePlayer(Player& player);

		bool addStatistics(Statistics& statistics);
		std::vector<Statistics> getStatistics(int pid,int type,int limit);
};

extern Database *_database;

#endif

/**/
