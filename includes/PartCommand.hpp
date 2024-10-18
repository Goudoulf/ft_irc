
#pragma once
#include "Command.hpp"
#include <string>

class PartCommand : public Command {

public:

    PartCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
    void	partChannel(std::string channel, std::string message, int fd, IRCServer &server);
};
