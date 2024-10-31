#ifndef WHOCOMMAND_HPP
#define WHOCOMMAND_HPP

#include "Command.hpp"

#include <string>

class WhoCommand : public Command {

public:

    WhoCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
