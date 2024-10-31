#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"

#include <string>
#include <vector>

class Client;

class Command {
public:
	virtual ~Command() {};
	virtual void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)= 0;
};

#endif
