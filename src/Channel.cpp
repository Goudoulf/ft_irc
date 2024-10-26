#include "Channel.hpp"
#include "debug.h"
#include <string>
#include <iostream>

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

void	Channel::add_client(Client *client)
{
	_users.insert(std::pair<Client*, bool>(client, false));	
}

channelMode	Channel::selectMode(std::string name)
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

std::string	Channel::createChannelId(time_t timestamp)
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

bool	Channel::keyIsValid(std::string &key)
{
	return (key == _password);
}

bool	Channel::inChannel(std::string client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		log(DEBUG,"inChannel: user=" + it->first->getNickname());
		if (it->first->getNickname() == client)
			return (true);
	}
	return (false);
}

void	Channel::remove_client(Client *client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end();) {
		if (it->first->getNickname() == client->getNickname())
			it = _users.erase(it);
		else
			++it;
	}
	if (_users.empty())
		_isEmpty = true;
}

void	Channel::sendMessage(Client *sender, std::string message)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first && it->first!= sender)
			sender->sendMessage(it->first->getSocket(), message);
	}
}

void	Channel::sendMessageWithSender(Client *sender, std::string message)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first)
			sender->sendMessage(it->first->getSocket(), message);
	}
}

void	Channel::sendReply(std::string message)
{
	IRCServer *server = IRCServer::getInstance();
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first)
			server->sendReply(it->first->getSocket(), message);
	}
}

void	Channel::addInvitation(Client *client)
{
	_invited.push_back(client);
}

bool	Channel::isOp(std::string client)
{
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first->getNickname() == client && it->second == true)
			return (true);
	}
	return (false);
}

std::string Channel::getUsers()
{
	std::string temp;
	for (std::map<Client*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it != _users.begin())
			temp += " ";
		if (isOp(it->first->getNickname()) == true)
			temp += "@" + it->first->getNickname();
		else
			temp += it->first->getNickname();
	}
	return temp;
}

void	Channel::setOp(Client *client, bool sign)
{
	std::map<Client*, bool>::iterator it = _users.find(client);
	if (it != _users.end())
		it->second = sign;
}

void	Channel::setPassword(std::string newPassword) {_password = newPassword;}

void	Channel::setTopic(std::string topic) {_topic = topic;}

void	Channel::setInviteOnly(bool sign) {_InviteOnly = sign;}

void	Channel::setIsTopicForOp(bool sign) {_isTopicForOp = sign;}

void	Channel::setIsLimited(bool sign) {_isLimited = sign;}

void	Channel::setLimitSize(unsigned int limit) {_limitSize = limit;}

channelMode Channel::getChannelMode()const {return _mode;}

std::string Channel::getChannelName()const {return _name;}

std::string Channel::getPassword()const {return _password;}

std::string		Channel::getTopic()const {return _topic;}

bool	Channel::getIsEmpty()const {return _isEmpty;}

bool	Channel::getInviteOnly(void)const {return _InviteOnly;}

bool	Channel::getIsTopicForOp(void)const {return _isTopicForOp;}

bool	Channel::getIsLimited(void)const {return _isLimited;}

unsigned int			Channel::getLimitSize()const {return _limitSize;}

std::map<Client*, bool> Channel::getUsersMap()const {return _users;}

std::vector<Client *>	Channel::getInvitationList()const {return _invited;}
