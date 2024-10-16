#include "Channel.hpp"
#include <string>
#include <iostream>

// TODO: Add fucntion to add new client to channel
bool	validName(std::string name)
{
	if (name.length() >= 50
		|| !(name.at(0) == '&'
			|| name.at(0) == '+'
			|| name.at(0) == '!'
			|| name.at(0) == '#')
		|| name.find(",") != (size_t)-1
		|| name.find(";") != (size_t)-1
		|| name.find("\a") != (size_t)-1)
		return (false);
	return (true);
}

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

Channel::Channel(const std::string &name, const Client &creator, const std::string &key)
{
	_name = name;
	_mode = selectMode(name);
	if (_mode == safe)
	{
		time_t timestamp = time(NULL);
		std::cout << "time: " << timestamp << std::endl;
		std::cout << "channelID: " << createChannelId(timestamp) << std::endl;
	}
	_users.insert(std::pair<Client, bool>(creator, true));
	_password = key;
	_isEmpty = false;
	_isInviteForOp = false;
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
	for (std::map<Client, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it != _users.begin())
			temp += " ";
		if (IsOp(it->first.GetNickname()) == true)
			temp += "@" + it->first.GetNickname();
		else
			temp += it->first.GetNickname();
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
	for (std::map<Client, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first.GetNickname() == client)
			return (true);
	}
	return (false);
}

bool	Channel::IsOp(std::string client)
{
	for (std::map<Client, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->first.GetNickname() == client && it->second == true)
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

void	Channel::add_client(Client &client)
{
	_users.insert(std::pair<Client, bool>(client, false));	
}

void	Channel::remove_client(Client &client)
{
	for (std::map<Client, bool>::iterator it = _users.begin(); it != _users.end();) {
		if (it->first.GetNickname() == client.GetNickname())
			it = _users.erase(it);
		else
			++it;
	}
	if (_users.empty())
		_isEmpty = true;
}

void	Channel::setIsInviteForOp(bool sign) {_isInviteForOp = sign;}

bool	Channel::getIsInviteForOp(void) {return _isInviteForOp;}