#pragma once
#include "Command.hpp"
#include <string>

class CapCommand : public Command {

public:

    CapCommand() {}
	void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
