
#pragma once
#include "Command.hpp"
#include <string>

class QuitCommand : public Command {

public:

    QuitCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
