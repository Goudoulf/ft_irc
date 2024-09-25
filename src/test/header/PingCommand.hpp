
#pragma once
#include "Command.hpp"
#include <string>

class PingCommand : public Command {

public:

    PingCommand() {}
    void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server);
};
