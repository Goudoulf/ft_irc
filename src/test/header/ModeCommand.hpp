
#pragma once
#include "Command.hpp"
#include <string>

class ModeCommand : public Command {

public:

    ModeCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
