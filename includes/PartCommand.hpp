
#pragma once
#include "Command.hpp"
#include <string>

class PartCommand : public Command {

public:

    PartCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string>>& params);
};
