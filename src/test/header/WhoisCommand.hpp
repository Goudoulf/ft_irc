
#pragma once
#include "Command.hpp"
#include <string>

class WhoisCommand : public Command {

public:

    WhoisCommand() {}
	void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server);
};
