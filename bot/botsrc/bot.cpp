#include "../bot.hpp"

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

Game *createHang(std::string type, std::vector<std::string> params)
{
	return (new HangMan(type, params));
}

Game *createConnectFour(std::string type, std::vector<std::string> params)
{
	return (new ConnectFour(type, params));
}

std::map<std::string, Game *(*)(std::string, std::vector<std::string>)> init_map()
{
	std::map<std::string, Game *(*)(std::string, std::vector<std::string>)> gameMap;
	gameMap.insert(std::make_pair(std::string("hangman"), &createHang));
	gameMap.insert(std::make_pair(std::string("connectfour"), &createConnectFour));
	return gameMap;
}

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
	_gameMap = init_map();
}


Bot::~Bot()
{
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
	if (game.at(0) == '!' && channel == "#botchan")
	{
		game = game.substr(1);
		std::getline(iss, trailing);
		std::vector<std::string> params = split (trailing.substr(1), ' ');
		addGame(game, params);
		return ;
	}
	Game *actualGame = findGame(channel);
	if (actualGame)
	{
		std::cout << "play the game" << std::endl;
	}
	else
		std::cout << "nothing" << std::endl;
}

void Bot::addGame(std::string game, std::vector<std::string> params)
{
	_games.push_back(_gameMap.find(game)->second(game, params));
	std::string joinMessage("JOIN #" + _games.back()->getChanName() + "\r\n");
	send(_socketFd, joinMessage.c_str(), joinMessage.length(), 0);
	std::string creationMessage("PRIVMSG #botchan :" + _games.back()->getChanName() + " created, have fun !\r\n");
	send(_socketFd, creationMessage.c_str(), creationMessage.length(), 0);
	std::cout << _games.back()->getChanName() + " added" << std::endl;
	std::vector<std::string> players = _games.back()->getPlayers();
	for (std::vector<std::string>::iterator it = players.begin(); it != players.end(); it++)
	{
		std::string inviteMessage("INVITE " + *it + " #" + _games.back()->getChanName() + "\r\n");
		send(_socketFd, inviteMessage.c_str(), inviteMessage.length(), 0);
	}
}

Game *Bot::findGame(std::string toFind)
{
	for (std::vector<Game*>::iterator it = _games.begin(); it != _games.end(); it++)
	{
		if ((*it)->getChanName() == toFind)
		{
			std::cout << "found" << std::endl;
			return (*it);
		}
	}
	std::cout << "not found" << std::endl;
	return NULL;
}