#pragma once
#include "Command.hpp"
#include <string>

class NamesCommand : public Command {

public:
    NamesCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};
