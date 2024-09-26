#include "../header/CommandDirector.hpp"
#include "../../../includes/debug.h"
#include <utility>
#include <sstream>
#include <string>
#include <map>

void	CommandDirector::addCommand(const TemplateBuilder *command)
{
	_commandList.insert(std::make_pair(command->getName(), command));
}

void	CommandDirector::parseCommand(int fd, std::string buffer, IRCServer& server)
{
	(void)fd;
    log(INFO, "Director Parsing");
    std::string trimmedMessage = buffer;
    std::istringstream iss(trimmedMessage);
    std::string prefix, command, params;

    trimmedMessage.erase(0, trimmedMessage.find_first_not_of(" \r\n"));
    trimmedMessage.erase(trimmedMessage.find_last_not_of(" \r\n") + 1);

    if (trimmedMessage.empty())
	return;
    if (trimmedMessage[0] == ':'){
	iss >> prefix;  
	prefix = prefix.substr(1);
    }
    iss >> command;
    if (command.empty())
    {
	// log(ERROR, "Received an invalid IRC message with no command.");
	return;
    }
    if ((_commandList.find(command)) == _commandList.end())
    {
	log(ERROR, "Can't find command " + command);
	return;
    }
    if (command == "CAP")
    {
	log(ERROR, "Cap do nothing");
	return ;
    }
    std::vector<std::string> parsedParams;
    std::string param;

    while (iss >> param)
    {
	if (param[0] == ':')
	{
	    std::string trailing;
	    std::getline(iss, trailing);
	    parsedParams.push_back(param.substr(1) + trailing);
	    break;
	} 
	parsedParams.push_back(param);
    }
    log(INFO, "Director fill param");
    _commandList.find(command)->second->fill_param(fd, parsedParams, server);
}
