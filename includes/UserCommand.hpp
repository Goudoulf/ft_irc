
#pragma once
#include "Command.hpp"
#include <string>

class UserCommand : public Command {

public:

    UserCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
