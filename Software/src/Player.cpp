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
#include "Player.h"

Player::Player()
{
	LOG_DEBUG("-");
}

Player::Player(int pid,std::string& name) : Player()
{
	LOG_DEBUG("pid=%d  name=%s",pid,name);

	this->pid = pid;
	this->name = name;
}

void Player::clear(void)
{
	pid = -1;
	name = "";
}

int Player::getPid(void)
{
	LOG_DEBUG("-");

	return pid;
}

void Player::setPid(int pid)
{
	LOG_DEBUG("pid=%d",pid);

	this->pid = pid;
}

std::string& Player::getName(void)
{
	//LOG_DEBUG("-");

	return name;
}

void Player::setName(const char *name)
{
	//LOG_DEBUG("%s",std::string(name));

	this->name = name;
}

void Player::setName(std::string& name)
{
	//LOG_DEBUG("%s",name);

	this->name = name;
}

bool Player::isValid(bool asNew)
{
	//LOG_DEBUG("asNew=%d",asNew);

	if (asNew && pid != -1)
		return false;
	return !name.empty();
}

Player::~Player()
{
	LOG_DEBUG("");
}/**/
