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
#include "Config.h"

Config::Config()
{
	LOG_DEBUG("-");

	/*
	**	construct the config file path
	*/
	configFile = Poco::Path::configHome();
	LOG_DEBUG("configHome=%s",configFile.toString());
	configFile.append("/" PACKAGE_NAME "/");
	configFile.setFileName("config.xml");
	LOG_DEBUG("configFile=%s",configFile.toString());

	/*
	**	create the path to the config file is neccessary
	*/
	Poco::File path(configFile.parent());
	path.createDirectories();

	loadConfig();

	setConfigString("Title",PACKAGE_NAME);
}

std::string Config::getConfigDir(void)
{
	LOG_DEBUG("-");

	LOG_DEBUG("configdir=%s",configFile.parent().toString());

	return configFile.parent().toString();
}

bool Config::loadConfig(void)
{
	LOG_DEBUG("-");

	config = new Poco::Util::XMLConfiguration();

	LOG_DEBUG("configFile=%s",configFile.toString());
	try {
		config->load(configFile.toString());
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	return false;
}

bool Config::saveConfig(void)
{
	LOG_DEBUG("-");

	LOG_DEBUG("configFile=%s",configFile.toString());
	try {
		config->save(configFile.toString());
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	return false;
}

int Config::getConfigInt(std::string key)
{
	LOG_DEBUG("key=%s",key);

	return getConfigInt(key,0);
}

int Config::getConfigInt(std::string key,int defaultValue)
{
	LOG_DEBUG("key=%s  defaultValue=%d",key,defaultValue);

	try {
		return config->getInt(key);
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	LOG_DEBUG("returning defaultValue=%d",defaultValue);

	return defaultValue;
}

std::string Config::getConfigString(std::string key)
{
	LOG_DEBUG("key=%s",key);

	return getConfigString(key,"");
}

std::string Config::getConfigString(std::string key,std::string defaultValue)
{
	LOG_DEBUG("key=%s  defaultValue=%s",key,defaultValue);

	try {
		return config->getString(key);
	}
	catch (Poco::Exception& exc) {
		LOG_ERROR("%s",exc.displayText());
	}

	return defaultValue;
}

bool Config::setConfigInt(std::string key,int value)
{
	LOG_DEBUG("key=%s  value=%d",key,value);

	config->setInt(key,value);
	saveConfig();

	return false;
}

bool Config::setConfigString(std::string key,std::string value)
{
	LOG_DEBUG("key=%s  value=%s",key,value);

	config->setString(key,value);
	saveConfig();

	return false;
}

Config::~Config()
{
	LOG_DEBUG("-");
}/**/
