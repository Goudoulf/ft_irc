
#pragma once
#include "Command.hpp"
#include <string>

class KickCommand : public Command {

public:

    KickCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
