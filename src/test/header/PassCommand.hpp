
#pragma once
#include "Command.hpp"
#include <string>

class PassCommand : public Command {

public:

    PassCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
