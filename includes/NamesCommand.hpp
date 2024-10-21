#pragma once
#include "Command.hpp"
#include <string>

class NamesCommand : public Command {

public:
    NamesCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
