#ifndef CAPCOMMAND_HPP
#define CAPCOMMAND_HPP

#include "Command.hpp"
#include <string>

class CapCommand : public Command {

public:

    CapCommand() {}
	void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
