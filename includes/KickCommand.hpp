
#pragma once
#include "Command.hpp"
#include <string>

class KickCommand : public Command {

public:

    KickCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
