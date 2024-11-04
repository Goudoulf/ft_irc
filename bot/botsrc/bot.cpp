#include "../bot.hpp"
#include "../debug.h"

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial)
{
    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(buffer);  

    while (std::getline(stream, temp))
    {
        if (!temp.empty() && temp[temp.size() - 1] == '\r')
            temp.erase(temp.size() - 1);
        lines.push_back(temp);
    }
    if (!buffer.empty() && buffer[buffer.size() - 1] != '\n')
    {
        if (!lines.empty())
        {
            remainingPartial = lines.back();
            lines.pop_back(); 
        }
        else
            remainingPartial = buffer; 
    }
    else
        remainingPartial.clear(); 
    return lines;
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

bool	errorExit(std::string message)
{
	log(ERROR, message);
	return (false);
}

bool	Bot::initialize(std::string port, std::string password)
{
	char *end;
	int opt = 1;

	_socketFd = -1;
	memset(&_address, 0, sizeof(_address));
	_port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
	if ((_socketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (errorExit(strerror(errno)));
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		return (errorExit(strerror(errno)));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	if (connect(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		return (errorExit(strerror(errno)));
	_gameMap = init_map();
	std::string passwordMessage = "PASS " + password + "\r\n"; 
	send(_socketFd, passwordMessage.c_str(), passwordMessage.length(), 0);
	send(_socketFd, "NICK bot\r\n", 10, 0);
	send(_socketFd, "USER bot 0 * :realname\r\n", 24, 0);
	usleep (1500);
	char buffer[1024];
	int valread = recv(_socketFd, buffer, 1024, 0);
	std::string stringBuffer(buffer);
	if (valread <= 0)
		return (errorExit("CONNECTION CLOSED"));
	return (true);
}

Bot::Bot() {}

Bot::~Bot()
{
	log(INFO, "Bot is disconnecting");
	for (std::vector<Game*>::iterator it = _games.begin(); it != _games.end(); it++)
		delete (*it);
	if (_socketFd != -1)
		close(_socketFd);
}

void Bot::run()
{
	char buffer[1024];
	int valread = 0;
	
	send(_socketFd, "JOIN #botchan\r\n", 15, 0);
	while (1)
	{
		bzero(buffer, 1024);
		valread = recv(_socketFd, buffer, 1024, 0);
		if (valread <= 0)
		{
			log (ERROR, "CONNECTION CLOSED");
			return;
		}
		std::string temp = "";
		std::vector<std::string> buf = splitBuffer(buffer, temp);
		for (std::vector<std::string>::iterator it = buf.begin(); it != buf.end(); it++)
		{
			log(DEBUG, buffer);
			if (!readData (*it))
			{
				log (ERROR, "CONNECTION CLOSED");
				return;
			}
		}
	}
}

std::vector<std::string> Bot::getPlayersList(std::string chanName)
{
	char buffer[1024];
	bzero(buffer, 1024);
	std::string toSend("NAMES " + chanName + "\r\n");
	send(_socketFd, toSend.c_str(), toSend.length(), 0);
	recv(_socketFd, buffer, 1024, 0);
	std::cout << "NAMES LIST = " << buffer << std::endl;
	std::vector<std::string> list;
	std::string line(buffer);
	if (line.empty())
		return list;
	line.erase(0, 1);
	line.erase(0, line.find_first_of(":", 0));
	line.erase(line.find_first_of("\r\n"), std::string::npos);
	line.erase(0, 1);
	std::istringstream iss(line);
	std::string param;
	while (iss >> param)
	{
		if (param.find("@bot") == std::string::npos)
			list.push_back(param);
	}
	return list;
}

bool	Bot::processPrivMSG(std::string command, std::string trailing)
{
	if (command == "433" || command == "451")
	{
		log (ERROR, "Invalid details");
		return (false);
	}
	if (command == "353")
	{
		trailing.erase(0, trailing.find_first_of(':'));
		std::vector<std::string> clients = split(trailing.substr(1), ' ');
		for (std::vector<std::string>::iterator _it = clients.begin(); _it != clients.end(); _it++)
		{
			if (*_it == "@bot")
				return (true);
		}
		usleep(1500);
		log (ERROR, "Not operator");
		return (false);
	}
	return (true);
}

bool Bot::readData (std::string buffer)
{
	std::string prefix, command, channel, game, trailing;

	buffer.erase(0, buffer.find_first_not_of(" \r\n"));
	buffer.erase(buffer.find_last_not_of(" \r\n") + 1);
	std::istringstream iss(buffer);
	iss >> prefix;
	if (prefix.empty())
		return false;
	prefix = prefix.substr(1);
	std::string client(prefix);
	if (client.find('!') != std::string::npos)
		client.erase(client.find('!'));
	iss >> command;
	if (command != "PRIVMSG")
	{
		std::getline(iss, trailing);
		return (processPrivMSG(command, trailing));
	}
	iss >> channel;
	iss >> game;
	game = game.substr(1);
	if (game.at(0) == '!' && channel == "#botchan")
	{
		game = game.substr(1);
		std::getline(iss, trailing);
		if (trailing.empty())
		{
			std::string errorMessage("PRIVMSG #botchan :not enough players to play\r\n");
			log(WARN, errorMessage);
			send(_socketFd, errorMessage.c_str(), errorMessage.length(), 0);
			return (true);
		}
		std::vector<std::string> params = split (trailing.substr(1), ' ');
		params.push_back(client);
		addGame(game, params);
	}
	else
	{
		Game *actualGame = findGame(channel);
		if (actualGame != NULL && !actualGame->isFinished())
		{
			actualGame->setCurrentPlayer(client);
			actualGame->setInput(game);
			if (game == "!start" && !actualGame->isStarted())
				actualGame->setPlayers(getPlayersList(actualGame->getChanName()));
			actualGame->gameLoop();
			std::vector<std::string> message = split(actualGame->getBuffer(), '\n');
			for (std::vector<std::string>::iterator it = message.begin(); it != message.end(); it++)
			{
				usleep(5000);
				if (actualGame->getBuffer().empty())
					break;
				std::string toSend("PRIVMSG " + actualGame->getChanName() + " :" + (*it).erase(((*it).length())) + "\r\n");
				send(_socketFd, toSend.c_str(), toSend.length(), 0);
			}
			actualGame->cleanBuffer();
		}
	}
	return (true);
}

void Bot::addGame(std::string game, std::vector<std::string> params)
{
	std::map<std::string, Game *(*)(std::string, std::vector<std::string>)>::iterator toAdd = _gameMap.find(game);
	if (toAdd == _gameMap.end())
	{
		std::string errorMessage("PRIVMSG #botchan :" + game + " doesn't exist\r\n");
		log(WARN, errorMessage);
		send(_socketFd, errorMessage.c_str(), errorMessage.length(), 0);
		return ;
	}
	std::vector<std::string> playersInLobby = getPlayersList("#botchan");
	std::vector<std::string> uniquePlayers;
	for (std::vector<std::string>::iterator _it = params.begin(); _it != params.end(); _it++)
	{
		bool found = false;
		for (std::vector<std::string>::iterator _it2 = playersInLobby.begin(); _it2 != playersInLobby.end() && !found; _it2++)
		{
			if ((*_it).compare(*_it2) == 0)
				found = true;
		}
		if (!found)
		{
			std::string errorMessage("PRIVMSG #botchan :" + *_it + " is not there\r\n");
			log(WARN, errorMessage);
			send(_socketFd, errorMessage.c_str(), errorMessage.length(), 0);
			return ;
		}
		for (std::vector<std::string>::iterator _it3 = uniquePlayers.begin(); _it3 != uniquePlayers.end(); _it3++)
		{
			if (*_it3 == *_it)
			{
				std::string errorMessage ("PRIVMSG #botchan :" + *_it + " can't invite the same player multiple times !\r\n");
				log(WARN, errorMessage);
				send (_socketFd, errorMessage.c_str(), errorMessage.length(), 0);
				return ;
			}
		}
		uniquePlayers.push_back(*_it);
	}
	_games.push_back(toAdd->second(game, params));
	std::string joinMessage("JOIN " + _games.back()->getChanName() + "\r\n");
	send(_socketFd, joinMessage.c_str(), joinMessage.length(), 0);
	std::string modeMessage("MODE " + _games.back()->getChanName() + " +i\r\n");
	send(_socketFd, modeMessage.c_str(), modeMessage.length(), 0);
	std::string creationMessage("PRIVMSG #botchan :" + _games.back()->getChanName() + " created, have fun !\r\n");
	send(_socketFd, creationMessage.c_str(), creationMessage.length(), 0);
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
		if ((*it)->getChanName() == toFind)
			return (*it);
	}
	return NULL;
}
