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

Game *createMorpion(std::string type, std::vector<std::string> params)
{
	return (new Morpion(type, params));
}

std::map<std::string, Game *(*)(std::string, std::vector<std::string>)> init_map()
{
	std::map<std::string, Game *(*)(std::string, std::vector<std::string>)> gameMap;
	gameMap.insert(std::make_pair(std::string("hangman"), &createHang));
	gameMap.insert(std::make_pair(std::string("connectfour"), &createConnectFour));
	gameMap.insert(std::make_pair(std::string("morpion"), &createMorpion));
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
	
	send(_socketFd, "PASS tutu\r\n", 11, 0);
	send(_socketFd, "NICK bot\r\nUSER bot 0 * :realname\r\n", 34, 0);
	send(_socketFd, "JOIN #botchan\r\n", 15, 0);
	while (1)
	{
		bzero(buffer, 1024);
		recv(_socketFd, buffer, 1024, 0);
		std::cout << buffer << std::endl;
		readData (buffer);
	}
}

std::vector<std::string> Bot::getPlayersList(std::string chanName)
{
	char buffer[1024];
	std::string toSend(":bot!bot@localhost NAMES " + chanName +"\r\n");
	send(_socketFd, toSend.c_str(), toSend.length(), 0);
	bzero(buffer, 1024);
	recv(_socketFd, buffer, 1024, 0);
	std::vector<std::string> list;
	std::string line(buffer);
	line.erase(line.find_first_of('\r'), std::string::npos);
	line.erase(0, 1);
	std::istringstream iss(line);
	std::string param;

	while (iss >> param)
	{
		if (param[0] == ':')
		{
			if (param != ":@bot" && param != ":bot")
			{
				param.erase(0);
				list.push_back(param);
			}
			while (iss >> param)
				list.push_back(param);
		}
	}
	
	return list;
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
	else
	{
		Game *actualGame = findGame(channel);
		if (actualGame != NULL && !actualGame->isFinished())
		{
			std::string client(prefix);
			client.erase(client.find('!'), std::string::npos);
			actualGame->setCurrentPlayer(client);
			actualGame->setInput(game);
			if (game == "!start" && !actualGame->isStarted())
				actualGame->setPlayers(getPlayersList(actualGame->getChanName()));
			actualGame->gameLoop();
			std::cout << actualGame->getBuffer() << std::endl;
			std::vector<std::string> message = split(actualGame->getBuffer(), '\n');
			for (std::vector<std::string>::iterator it = message.begin(); it != message.end(); it++)
			{
				usleep(5000);
				if (actualGame->getBuffer().empty())
					break;
				std::string toSend("PRIVMSG " + actualGame->getChanName() + " :" + (*it).erase(((*it).length())) + "\r\n");
				std::cout << toSend<< std::endl;
				send(_socketFd, toSend.c_str(), toSend.length(), 0);
			}
			std::cout << actualGame->getBuffer().length() << std::endl;
			actualGame->cleanBuffer();
			if (actualGame->isFinished())
				return;
		}
		else
			std::cout << "nothing" << std::endl;
	}
}

void Bot::addGame(std::string game, std::vector<std::string> params)
{
	std::map<std::string, Game *(*)(std::string, std::vector<std::string>)>::iterator toAdd = _gameMap.find(game);
	if (toAdd == _gameMap.end())
	{
		std::string errorMessage("PRIVMSG #botchan :" + game + " doesn't exist\r\n");
		send(_socketFd, errorMessage.c_str(), errorMessage.length(), 0);
		return ;
	}
	_games.push_back(toAdd->second(game, params));
	std::string joinMessage("JOIN " + _games.back()->getChanName() + "\r\n");
	send(_socketFd, joinMessage.c_str(), joinMessage.length(), 0);
	std::string creationMessage("PRIVMSG #botchan :" + _games.back()->getChanName() + " created, have fun !\r\n");
	send(_socketFd, creationMessage.c_str(), creationMessage.length(), 0);
	std::cout << _games.back()->getChanName() + " added" << std::endl;
	std::vector<std::string> players = _games.back()->getPlayers();
	for (std::vector<std::string>::iterator it = players.begin(); it != players.end(); it++)
	{
		std::string inviteMessage("INVITE " + *it + " " + _games.back()->getChanName() + "\r\n");
		send(_socketFd, inviteMessage.c_str(), inviteMessage.length(), 0);
	}
}

Game *Bot::findGame(std::string toFind)
{
	for (std::vector<Game*>::iterator it = _games.begin(); it != _games.end(); it++)
	{
		std::cout << "iter" << std::endl;
		if ((*it)->getChanName() == toFind)
		{
			std::cout << "found" << std::endl;
			return (*it);
		}
	}
	std::cout << "not found" << std::endl;
	return NULL;
}