#pragma once
#include <string>
#include "IRCServer.hpp"
#include "cmds.h"

class IRCServer;

class Command {
public:
	virtual ~Command() {};
	virtual void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)= 0;
};
