
#pragma once
#include "Command.hpp"
#include <string>

class PingCommand : public Command {

public:

    PingCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
