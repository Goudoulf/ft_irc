#pragma once
#include "Command.hpp"
#include <string>

class PongCommand : public Command {

public:

    PongCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
