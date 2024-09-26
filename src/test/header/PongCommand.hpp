#pragma once
#include "Command.hpp"
#include <string>

class PongCommand : public Command {

public:

    PongCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
