#pragma once
#include <string>
#include "IRCServer.hpp"
#include "Client.hpp"
#include <vector>
#include "cmds.h"

class IRCServer;
class Client;

class Command {
public:
	virtual ~Command() {};
	virtual void execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)= 0;
};
