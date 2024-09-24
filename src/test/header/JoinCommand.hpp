#pragma once
#include "Command.hpp"
#include <string>

class JoinCommand : public Command {

public:

    JoinCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
