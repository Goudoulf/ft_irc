#ifndef NICKCOMMAND_HPP
#define	NICKCOMMAND_HPP

#include "Command.hpp"
#include <string>

class Command;

class NickCommand : public Command {
public:

    NickCommand() {} 
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
