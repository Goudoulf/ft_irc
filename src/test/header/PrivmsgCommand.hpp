
#pragma once
#include "Command.hpp"
#include <string>

class PrivmsgCommand : public Command {

public:

    PrivmsgCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
