#include "Channel.hpp"

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

Channel::Channel(const std::string &name, const Client &creator)
{
	if (!validName(name))
		throw InvalidName();
	_name = name;
	_mode = selectMode(name);
	//_users.insert(std::make_pair(creator, true));
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

