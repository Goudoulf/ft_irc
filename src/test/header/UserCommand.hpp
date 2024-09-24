
#pragma once
#include "Command.hpp"
#include <string>

class UserCommand : public Command {

public:

    UserCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
