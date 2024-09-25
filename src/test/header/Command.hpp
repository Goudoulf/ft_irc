#pragma once
#include <string>
#include <vector>
#include "../../IRCServer.hpp"


class Command {
public:
	virtual ~Command() {};
	virtual void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server)= 0;
};
