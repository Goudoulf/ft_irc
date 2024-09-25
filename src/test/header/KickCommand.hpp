
#pragma once
#include "Command.hpp"
#include <string>

class KickCommand : public Command {

public:

    KickCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
