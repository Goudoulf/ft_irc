
#pragma once
#include "Command.hpp"
#include <string>

class InviteCommand : public Command {

public:

    InviteCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string>>& params);
};
