#include "Channel.hpp"
#include <string>

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
	if (!validName(name))
		throw InvalidName();
	_name = name;
	_mode = selectMode(name);
	if (_mode == safe)
	{
		time_t timestamp = time(NULL);
		std::cout << "time: " << timestamp << std::endl;
		std::cout << "channelID: " << createChannelId(timestamp) << std::endl;
	}
	_operators.push_back(creator);
	_users.push_back(creator);
	_password = key;
	_isEmpty = false;
	_isInviteOnly = false;
	_topicRestrictions = false;
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

std::string Channel::getUsers()
{
	std::string temp;
	std::vector<Client>::iterator it;
	for (it = _users.begin(); it != _users.end(); it++) {
		if (isOp(*it) == true)
			temp = temp + "@" + it->GetNickname();
		else
			temp = temp + it->GetNickname();
		if (it->GetUsername() != _users.back().GetUsername())
			temp = temp + " ";
	}
	return temp;
}

bool	Channel::keyIsValid(std::string &key)
{
	if (key == _password)
		return true;
	return false;
}

bool	Channel::inChannel(std::string client)
{
	std::vector<Client>::iterator it;
	for (it = _users.begin(); it != _users.end(); it++) {
		if (it->GetUsername() == client)
			return (true);
	}
	return (false);
}

bool	Channel::isOp(Client &client)
{
	for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); it++) 
	{
		if (it->GetUsername() == client.GetUsername())
			return (true);
	}
	return (false);
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void Channel::setInviteOnly(bool sign)
{
	_isInviteOnly = sign;
}

void Channel::addOp(Client &client)
{
	_operators.push_back(client);
}

bool Channel::getInviteOnly()
{
	return _isInviteOnly;
}

bool Channel::getTopicRestrictions()
{
	return _topicRestrictions;
}

void Channel::setTopicRestrictions(bool sign)
{
	_topicRestrictions = sign;
}

int Channel::getClientLimit()
{
	return _clientLimit;
}

void Channel::setClientLimit(int limit)
{
	_clientLimit = limit;
}

std::string	Channel::getTopic()
{
	return _topic;
}

void	Channel::addClient(Client &client)
{
	_users.push_back(client);
}

void	Channel::remove_client(Client &client)
{
	for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end();) {
		if (it->GetUsername() == client.GetUsername())
			it = _operators.erase(it);
		else
			++it;
	}
	for (std::vector<Client>::iterator it = _users.begin(); it != _users.end();) {
		if (it->GetUsername() == client.GetUsername())
			 it = _users.erase(it);
		else
			++it;
	}
	if (_users.empty() && _operators.empty())
		_isEmpty = true;
}
