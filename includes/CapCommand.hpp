#pragma once
#include "Command.hpp"
#include <string>

class CapCommand : public Command {

public:

    CapCommand() {}
	void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
