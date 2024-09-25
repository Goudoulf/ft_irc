
#pragma once
#include "Command.hpp"
#include <string>

class PrivmsgCommand : public Command {

public:

    PrivmsgCommand() {}
    void execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server);
};
