#include "CommandDirector.hpp"
#include "TemplateBuilder.hpp"

#include "debug.h"
#include "reply.h"

#include <sstream>
#include <map>

CommandDirector::~CommandDirector()
{
	log(INFO, "Destroying Command Director");
	std::map<std::string, TemplateBuilder *>::iterator it;
	for (it = _commandList.begin(); it != _commandList.end(); it++)
	{
		if (it->second)
			delete it->second;
	}
	_commandList.clear();
}

void	CommandDirector::addCommand(TemplateBuilder *command)
{
	_commandList.insert(std::make_pair(command->getName(), command));
}

void	CommandDirector::parseCommand(Client* client, std::string buffer)
{
    log(INFO, "Director Parsing");
    std::string trimmedMessage = buffer;
    std::istringstream iss(trimmedMessage);
    std::string prefix, command;

    trimmedMessage.erase(0, trimmedMessage.find_first_not_of(" \r\n"));
    trimmedMessage.erase(trimmedMessage.find_last_not_of(" \r\n") + 1);

    if (trimmedMessage.empty())
	    return;
    if (trimmedMessage[0] == ':')
    {
	    iss >> prefix;  
	    prefix = prefix.substr(1);
    }
    iss >> command;
    if (command.empty())
    {
	    log(ERROR, "Received an invalid IRC message with no command.");
	    return;
    }
    if ((_commandList.find(command)) == _commandList.end())
    {
	    rplSend(client->getSocket(), ERR_UNKNOWNCOMMAND(command));
	    return;
    }
	if (command == "CAP")
		return;
    log(INFO, "Director fill param");
    _commandList.find(command)->second->executeCommand(client, trimmedMessage);
}
