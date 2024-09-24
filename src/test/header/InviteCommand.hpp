
#pragma once
#include "Command.hpp"
#include <string>

class InviteCommand : public Command {

public:

    InviteCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
