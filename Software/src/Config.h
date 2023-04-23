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
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "common.h"

class Config {
	public:
		Config();
		~Config();

		std::string getConfigDir(void);

		int getConfigInt(std::string key);
		int getConfigInt(std::string key,int defaultValue);
		bool setConfigInt(std::string key,int value);

		std::string getConfigString(std::string key);
		std::string getConfigString(std::string key,std::string defaultValue);
		bool setConfigString(std::string key,std::string value);

	private:
		Poco::Path configFile;
		Poco::Util::XMLConfiguration *config = NULL;

		bool loadConfig(void);
		bool saveConfig(void);
};


extern Config *_config;

#endif

/**/
