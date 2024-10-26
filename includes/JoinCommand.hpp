#pragma once
#include "Command.hpp"
#include <string>

class JoinCommand : public Command {

public:
    JoinCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
