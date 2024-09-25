#pragma once
#include "Command.hpp"
#include <string>

class Command;

class NickCommand : public Command {
public:

    NickCommand() {} 
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
