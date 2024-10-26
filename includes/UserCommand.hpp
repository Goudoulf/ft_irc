
#pragma once
#include "Command.hpp"
#include <string>

class UserCommand : public Command {

public:

    UserCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
