#pragma once
#include "Command.hpp"
#include <string>

class WhoCommand : public Command {

public:

    WhoCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};