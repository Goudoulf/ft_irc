#include "CommandDirector.hpp"
#include "debug.h"
#include <utility>
#include <sstream>
#include <string>
#include <map>
#include <vector>

void	CommandDirector::addCommand(const TemplateBuilder *command)
{
	_commandList.insert(std::make_pair(command->getName(), command));
}

std::vector<std::string> parseMode(std::vector<std::string> parsedParams)
{
    std::vector<std::string> parsedMode;
    
    std::string temp;
    for (std::vector<std::string>::iterator it = parsedParams.begin(); it != parsedParams.end(); ++it)
    {
	if (it == parsedParams.begin())
	{
	    temp = *it;
	}
	else if ((*it)[0] == '+' || (*it)[0] == '-')
	{
	    parsedMode.push_back(temp);
	    temp.clear();
	    temp = *it;
	}
	else if ((*it)[0] != '+' && (*it)[0] != '-')
	    temp += " " + *it;
	if ((it + 1) == parsedParams.end())
	    parsedMode.push_back(temp);
    }
    std::cout << "MODE PARAM {" << std::endl;
    for (std::vector<std::string>::iterator it = parsedMode.begin(); it != parsedMode.end(); it++)
    {
	 std::cout << *it << std::endl;
    }
    std::cout << "}" << std::endl;
    return parsedMode;
}

void	CommandDirector::parseCommand(Client* client, std::string buffer)
{
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
	log(ERROR, "Received an invalid IRC message with no command.");
	return;
    }
    if ((_commandList.find(command)) == _commandList.end())
    {
	log(ERROR, "Can't find command " + command);
	return;
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
    if (command == "MODE")
	    parsedParams = parseMode(parsedParams);
    log(INFO, "Director fill param");
    _commandList.find(command)->second->fill_param(client, parsedParams);
}
