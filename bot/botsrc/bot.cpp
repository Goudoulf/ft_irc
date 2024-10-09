#include "bot.hpp"

Bot::Bot(std::string port)
{
	char *end;
	int test = 1;
	memset(&_address, 0, sizeof(_address));
	_port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    if ((_socketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
        std::cout << "ERROR socket" << std::endl;
	}
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &test, sizeof(test)))
	{
        std::cout << "ERROR socketopt" << std::endl;
	}
	_address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
	if (connect(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		std::cout << "ERROR CONNECT" << std::endl;
	}
}

Bot::~Bot()
{
}

Game *Bot::findGame(std::string toFind)
{
	for (std::vector<Game*>::iterator it = _games.begin(); it != _games.end(); it++)
	{
		if ((*it)->getChanName() == toFind)
			return (*it);
	}
	return NULL;
}
void Bot::run()
{
	char buffer[1024];
	
	sleep(1);
	send(_socketFd, "PASS tutu\r\n", 11, 0);
	sleep(1);
	send(_socketFd, "NICK bot\r\nUSER bot 0 * :realname\r\n", 34, 0);
	sleep(1);
	send(_socketFd, "JOIN #botchan\r\n", 15, 0);
	while (1)
	{
		bzero(buffer, 1024);
		recv(_socketFd, buffer, 1024, 0);
		std::cout << buffer << std::endl;
		readData (buffer);
	}
}

void Bot::readData (std::string buffer)
{
	std::istringstream iss(buffer);
	std::string prefix, command, channel, game, trailing;

	buffer.erase(0, buffer.find_first_not_of(" \r\n"));
    buffer.erase(buffer.find_last_not_of(" \r\n") + 1);
	iss >> prefix;
	prefix = prefix.substr(1);
	iss >> command;
	if (command != "PRIVMSG")
		return;
	iss >> channel;
	iss >> game;
	game = game.substr(1);
	if (game.at(0) != '!' && channel == "#botchan")
		return;
	std::getline(iss, trailing);
	std::vector<std::string> params = split (trailing.substr(1), ' ');
	std::cout << "game: " << game << std::endl;
	std::cout << "params: " << params.front() << std::endl;
	std::cout << "trailing: " << trailing << std::endl;
	findGame(game);
}

std::vector<std::string> split(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}