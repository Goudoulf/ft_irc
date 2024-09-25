
#pragma once
#include "Command.hpp"
#include <string>

class PartCommand : public Command {

public:

    PartCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
    void	partChannel(std::string channel, std::string message, int fd, IRCServer &server);
};
