#include "Channel.hpp"
#include "debug.h"
#include <string>
#include <iostream>

channelMode	selectMode(std::string name)
{
	char tab[4] = {'&', '+', '!', '#'};
	int index;
	for (index = 0; index < 4 && tab[index] != name.at(0); index++) {}
	switch (index)
	{
		case 0:
			return (local);
		case 1:
			return (noMode);
		case 2:
			return (safe);
		case 3:
			return (standard);
		default:
			break;
	}
	return (error);
}

std::string	createChannelId(time_t timestamp)
{
	std::string base36("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	std::string res;

	for (int i = 5; i > 0; i--)
	{
		res = base36.at(timestamp % 36) + res;
		timestamp /= 36;
	}
	return (res);
}

Channel::Channel(const std::string &name, Client *creator, const std::string &key)
{
	_name = name;
	_mode = selectMode(name);
	if (_mode == safe)
	{
		time_t timestamp = time(NULL);
		std::cout << "time: " << timestamp << std::endl;
		std::cout << "channelID: " << createChannelId(timestamp) << std::endl;
	}
	bool creatorOp = true;
	if (_mode == noMode)
		creatorOp = false;
	_users.insert(std::pair<Client*, bool>(creator, creatorOp));
	_password = key;
	_isEmpty = false;
	_InviteOnly = false;
	_isTopicForOp = false;
	_isLimited = false;
}

Channel::~Channel()
{
}

std::string Channel::getChannelName()
{
	return _name;
}

channelMode Channel::getChannelMode()
{
	return _mode;
}

std::string Channel::getPassword()
{
	return _password;
}

std::string Channel::getUsers()
{
	std::string temp;
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it != _users.begin())
			temp += " ";
		if (IsOp(it->first->GetNickname()) == true)
			temp += "@" + it->first->GetNickname();
		else
			temp += it->first->GetNickname();
	}
	return temp;
}

bool	Channel::keyIsValid(std::string &key)
{
	if (key == _password)
		return true;
	return false;
}

bool	Channel::InChannel(std::string client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		log(DEBUG,"InChannel: user=" + it->first->GetNickname());
		if (it->first->GetNickname() == client)
			return (true);
	}
	return (false);
}

bool	Channel::IsOp(std::string client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first->GetNickname() == client && it->second == true)
			return (true);
	}
	return (false);
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string		Channel::getTopic()
{
	return _topic;
}

bool	Channel::getIsEmpty(void)
{
	return _isEmpty;
}

void	Channel::add_client(Client *client)
{
	_users.insert(std::pair<Client*, bool>(client, false));	
}

void	Channel::remove_client(Client *client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end();) {
		if (it->first->GetNickname() == client->GetNickname())
			it = _users.erase(it);
		else
			++it;
	}
	if (_users.empty())
		_isEmpty = true;
}

void	Channel::setInviteOnly(bool sign) {_InviteOnly = sign;}

bool	Channel::getInviteOnly(void) {return _InviteOnly;}

void	Channel::setIsTopicForOp(bool sign) {_isTopicForOp = sign;}

bool	Channel::getIsTopicForOp(void) {return _isTopicForOp;}

void	Channel::setIsLimited(bool sign) {_isLimited = sign;}

bool	Channel::getIsLimited(void) {return _isLimited;}

void	Channel::setLimitSize(unsigned int limit) {_limitSize = limit;}

unsigned int		Channel::getLimitSize(void) {return _limitSize;}

std::map<Client*, bool> Channel::getUsersMap(void) {return _users;}

std::vector<Client *>	Channel::getInvitationList(){return _invited;}

void	Channel::sendMessage(Client *sender, std::string message)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first && it->first->GetNickname() != sender->GetNickname())
			sender->sendMessage(it->first->GetSocket(), message);
	}
}

void	Channel::sendReply(std::string message)
{
	IRCServer *server = IRCServer::getInstance();
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first)
			server->sendReply(it->first->GetSocket(), message);
	}
}

void	Channel::addInvitation(Client *client)
{
	_invited.push_back(client);
}
void	Channel::setPassword(std::string newPassword) {_password = newPassword;}

void	Channel::setOp(Client *client, bool sign)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first->GetNickname() == client->GetNickname())
		{
			it->second = sign;
			return;
		}
	}
}
