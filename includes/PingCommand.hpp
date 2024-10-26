
#pragma once
#include "Command.hpp"
#include <string>

class PingCommand : public Command {

public:

    PingCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
