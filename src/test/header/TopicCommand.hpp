
#pragma once
#include "Command.hpp"
#include <string>

class TopicCommand : public Command {

public:

    TopicCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params, IRCServer& server);
};
