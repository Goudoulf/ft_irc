
#pragma once
#include "Command.hpp"
#include <string>

class TopicCommand : public Command {

public:

    TopicCommand() {}
    void execute(Client *client, std::map<std::string, std::vector<std::string>>& params);
};
