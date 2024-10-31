#pragma once

#include "CommandDirector.hpp"
#include "Client.hpp"
#include <vector>
#include <string>
#include <iostream>

class IRCServer;
class Client;
class CommandDirector;

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial);
void	messageServer(std::string target, std::string command, Client &client, std::string message, int sd);
std::vector<std::string> split(const std::string& input, char delimiter);

bool	isValidNick(const std::string param, Client * client);
bool	isValidPassword(const std::string param, Client * client);
bool	checkIsEmpty(const std::string param, Client * client);
bool	isValidChannel(const std::string param, Client * client);
bool	channelExists(const std::string param, Client * client);
bool	isOnChannel(const std::string param, Client * client);
bool	nickExists(const std::string param, Client * client);
bool    isValidInvite(const std::string param, Client *client);
bool    isInLimits(const std::string param, Client *client);
bool    isInvited(const std::string param, Client *client);
bool    isValidMode(const std::string param, Client *client);
bool    checkIsOp(const std::string param, Client *client);
bool    channelSupportsMode(const std::string param, Client *client);

void    setCommandTemplate(CommandDirector *director);

std::string	getTime();
