
#pragma once
#include "Command.hpp"
#include <string>

class ModeCommand : public Command {

public:

    ModeCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
