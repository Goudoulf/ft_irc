#pragma once
#include "Command.hpp"
#include <string>

class WhoCommand : public Command {

public:

    WhoCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
