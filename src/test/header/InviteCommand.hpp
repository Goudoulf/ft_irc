
#pragma once
#include "Command.hpp"
#include <string>

class InviteCommand : public Command {

public:

    InviteCommand() {}
    void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server);
};
