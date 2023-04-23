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
#include "Database.h"

using namespace Poco::Data::Keywords;

Database::Database()
{
	LOG_DEBUG("-");

	players = 0;

    Poco::Data::SQLite::Connector::registerConnector();
	session = new Poco::Data::Session(DbBackend,_config->getConfigDir() + DbFile);

	/*
	**	create the schema if it doesn't exists yet
	*/
	for (int n = 0;_DbTables[n].DbTableName;n++) {
		/*
		**	check the table
		*/
		int exists = false;

		LOG_DEBUG("check table=%s for existance",std::string(_DbTables[n].DbTableName));

		try {
			*session << "SELECT EXISTS ( SELECT name FROM sqlite_schema WHERE type='table' AND name='" << _DbTables[n].DbTableName << "' )",
				into(exists),
				now;
		}
		catch (Poco::Exception& exc) {
			LOG_ERROR("%s",exc.displayText());
		}
		LOG_DEBUG("table=%s  exists=%d",std::string(_DbTables[n].DbTableName),exists);
		if (!exists) {
			/*
			**	drop the table
			*/
			try {
				LOG_WARNING("dropping table=%s",std::string(_DbTables[n].DbTableName));
				*session << "DROP TABLE IF EXISTS " << _DbTables[n].DbTableName << "" , now;
			}
			catch (Poco::Exception& exc) {
				LOG_ERROR("%s",exc.displayText());
			}

			/*
			**	create the table
			*/
			try {
				LOG_WARNING("creating table=%s",std::string(_DbTables[n].DbTableName));
				*session << "CREATE TABLE " << _DbTables[n].DbTableName << " ( " << _DbTables[n].DbTableDescription << ")", now;
			}
			catch (Poco::Exception& exc) {
				LOG_ERROR("%s",exc.displayText());
			}
		}
	}
}

int Database::getPlayers(void)
{
	LOG_DEBUG("-");

	try {
		*session << "SELECT COUNT(*) FROM " << DbTablePlayerName << "", into(players), now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	LOG_DEBUG("players=%d",players);

	return players;
}

std::vector<Player> Database::getAllPlayers(void)
{
	LOG_DEBUG("-");
	std::vector<Player> list;

	try {
		int pid;
		std::string name;

		Poco::Data::Statement select(*session);
		select << "SELECT pid,name FROM " << DbTablePlayerName << " ORDER BY name",
			into(pid),
			into(name),
			range(0, 1);

		while (!select.done()) {
			select.execute();
			list.push_back(Player(pid,name));
		}
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	return list;
}

Player Database::getPlayer(std::string& name)
{
	LOG_DEBUG("name=%s",name);

	int pid;

	try {
		*session << "SELECT pid,name FROM " << DbTablePlayerName << " WHERE name=?",
			use(name),
			into(pid),
			into(name),
			now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
		return Player();
	}

	return Player(pid,name);
}

bool Database::addPlayer(Player& player)
{
	LOG_DEBUG("name=%s",player.getName());

	lastInsertRowId = -1;

	try {
		*session << "INSERT INTO " << DbTablePlayerName << " (name) VALUES (?)", use(player.getName()), now;
		*session << "SELECT last_insert_rowid()", into(lastInsertRowId), now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
		lastInsertRowId = -1;
		return false;
	}

	LOG_DEBUG("lastInsertRowId=%d",lastInsertRowId);

	return true;
}

bool Database::modifyPlayer(Player& player)
{
	LOG_DEBUG("pid=%d  Player=%s",player.getPid(),player.getName());

	try {
		int pid = player.getPid();

		*session << "UPDATE " << DbTablePlayerName << " SET name = ? WHERE pid = ?", use(player.getName()), use(pid), now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
		return false;
	}

	return true;
}

bool Database::removePlayer(Player& player)
{
	return removePlayer(player.getPid());
}

bool Database::removePlayer(int pid)
{
	LOG_DEBUG("pid=%d",pid);

	try {
		*session << "DELETE FROM " << DbTablePlayerName << " WHERE pid = (?)", use(pid), now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
		return false;
	}

	return true;
}

bool Database::addStatistics(Statistics& statistics)
{
	LOG_DEBUG("pid=%d  type=%d  value=%5.2f",statistics.getPid(),statistics.getType(),statistics.getValue());

	lastInsertRowId = -1;

	try {
		int pid = statistics.getPid();
		int type = statistics.getType();
		double value = statistics.getValue();
		time_t timestamp = statistics.getTimestamp();

		*session << "INSERT INTO " << DbTableStatisticsName << " (pid,type,value,timestamp) VALUES (?,?,?,?)",
			use(pid),
			use(type),
			use(value),
			use(timestamp),
			now;
		*session << "SELECT last_insert_rowid()", into(lastInsertRowId), now;
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
		lastInsertRowId = -1;
		return false;
	}

	return true;
}

std::vector<Statistics> Database::getStatistics(int pid,int type,int limit)
{
	LOG_DEBUG("pid=%d  type=%d  limit=%d",pid,type,limit);
	std::vector<Statistics> list;

	double value;
	time_t timestamp;
	Poco::Data::Statement select(*session);
	try {
		select << "SELECT value,CAST(STRFTIME(\"%s\",timestamp) AS INT) FROM " << DbTableStatisticsName << " WHERE pid=? AND type=? ORDER BY Timestamp DESC LIMIT ?;",
			use(pid),
			use(type),
			use(limit),
			into(value),
			into(timestamp),
			range(0, 1);

		LOG_DEBUG("retrieving records ...");
		while (!select.done()) {
			select.execute();
			list.push_back(Statistics(pid,type,value,timestamp));
		}
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	LOG_DEBUG("pid=%d  type=%d  limit=%d  records retrieved=%d",pid,type,limit,(int) list.size());

	return list;
}

Database::~Database()
{
	LOG_DEBUG("-");
}/**/
