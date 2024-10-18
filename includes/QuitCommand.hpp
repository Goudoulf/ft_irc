
#pragma once
#include "Command.hpp"
#include <string>

class QuitCommand : public Command {

public:

    QuitCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
