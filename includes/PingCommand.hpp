
#pragma once
#include "Command.hpp"
#include <string>

class PingCommand : public Command {

public:

    PingCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
