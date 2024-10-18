#pragma once
#include "Command.hpp"
#include <string>

class Command;

class NickCommand : public Command {
public:

    NickCommand() {} 
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
