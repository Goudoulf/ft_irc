
#pragma once
#include "Command.hpp"
#include <string>

class PassCommand : public Command {

public:

    PassCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
