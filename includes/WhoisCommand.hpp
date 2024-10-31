#ifndef WHOISCOMMAND_HPP
#define WHOISCOMMAND_HPP

#include "Command.hpp"
#include <string>

class WhoisCommand : public Command {

public:

    WhoisCommand() {}
	void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
