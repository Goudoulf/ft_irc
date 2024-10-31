#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

#include "Command.hpp"

#include <string>
#include <vector>

class PrivmsgCommand : public Command {

public:

    PrivmsgCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
