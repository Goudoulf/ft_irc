#pragma once
#include <string>
#include "IRCServer.hpp"
#include "cmds.h"

class IRCServer;

class Command {
public:
	virtual ~Command() {};
	virtual void execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)= 0;
};
