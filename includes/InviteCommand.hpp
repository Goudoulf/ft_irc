
#pragma once
#include "Command.hpp"
#include <string>

class InviteCommand : public Command {

public:

    InviteCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};