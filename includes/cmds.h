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

bool	isAlphaNum(const std::string param, Client * client);
bool	isConnected(const std::string param, Client * client);
bool	isValidNick(const std::string param, Client * client);
bool	isValidPassword(const std::string param, Client * client);
bool	isEmpty(const std::string param, Client * client);
bool	isValidChannel(const std::string param, Client * client);
bool	ChannelExist(const std::string param, Client * client);
bool	isOnChannel(const std::string param, Client * client);
bool	nickExist(const std::string param, Client * client);
bool    isValidInvite(const std::string param, Client *client);
bool    isInLimits(const std::string param, Client *client);
bool    isInvited(const std::string param, Client *client);
bool    isValidMode(const std::string param, Client *client);
bool    checkiIsOp(const std::string param, Client *client);
bool    channelSupportsMode(const std::string param, Client *client);

void    setCommandTemplate(CommandDirector *director);

void setNonBlocking(int sockfd);
std::string	getTime();
