#ifndef KICKCOMMAND_HPP
#define	KICKCOMMAND_HPP

#include "Command.hpp"
#include <string>

class KickCommand : public Command {

public:

    KickCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
