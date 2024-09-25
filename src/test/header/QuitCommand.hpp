
#pragma once
#include "Command.hpp"
#include <string>

class QuitCommand : public Command {

public:

    QuitCommand() {}
    void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server);
};
