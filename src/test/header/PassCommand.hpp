
#pragma once
#include "Command.hpp"
#include <string>

class PassCommand : public Command {

public:

    PassCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
