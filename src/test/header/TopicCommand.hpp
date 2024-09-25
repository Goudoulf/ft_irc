
#pragma once
#include "Command.hpp"
#include <string>

class TopicCommand : public Command {

public:

    TopicCommand() {}
    void execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server);
};
