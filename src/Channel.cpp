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

Channel::Channel(const std::string &name, const Client &creator, const std::string &key)
{
	if (!validName(name))
		throw InvalidName();
	_name = name;
	_mode = selectMode(name);
	_operators.push_back(creator);
	_users.push_back(creator);
	_password = key;
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
		if (IsOp(it->GetUsername()) == true)
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

bool	Channel::InChannel(std::string client)
{
	std::vector<Client>::iterator it;
	for (it = _users.begin(); it != _users.end(); it++) {
		if (it->GetUsername() == client)
			return (true);
	}
	return (false);
}

bool	Channel::IsOp(std::string client)
{
	std::vector<Client>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++) {
		if (it->GetUsername() == client)
			return (true);
	}
	return (false);
}

std::string		Channel::getTopic()
{
	return _topic;
}

void	Channel::add_client(Client &client)
{
	_users.push_back(client);	
}

void	Channel::remove_client(Client &client)
{
	for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (it->GetUsername() == client.GetUsername())
			_operators.erase(it);
	}
	for (std::vector<Client>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (it->GetUsername() == client.GetUsername())
			_users.erase(it);
	}
}
