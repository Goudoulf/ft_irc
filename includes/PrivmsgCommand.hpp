
#pragma once
#include "Command.hpp"
#include <string>

class PrivmsgCommand : public Command {

public:

    PrivmsgCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
