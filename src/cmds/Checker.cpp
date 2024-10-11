#include "CmdLevel.h"
#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"

bool	isValidPassword(const std::string param, int fd , IRCServer& server)
{
    if (server.getPassword() == param)
	return true;
    rpl_send(fd, ERR_PASSWDMISMATCH());
    return false;
}

bool	isEmpty(const std::string param, int fd , IRCServer& server)
{
    (void)server;
    if (param.empty())
    {
	rpl_send(fd, ERR_NONICKNAMEGIVEN());
        return false;
    }
    return true;
}

bool	isValidNick(const std::string param, int fd , IRCServer& server)
{
    if (param.length() > 9)
    {
	rpl_send(fd, ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    char firstChar = param[0];
    if (!isalpha(firstChar) && firstChar != '-' && firstChar != '[' && firstChar != ']' &&
        firstChar != '\\' && firstChar != '^' && firstChar != '_' && firstChar != '{' &&
        firstChar != '}' && firstChar != '|') {
	rpl_send(fd, ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    for (size_t i = 1; i < param.length(); ++i) {
        char c = param[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']' &&
            c != '\\' && c != '^' && c != '_' && c != '{' &&
            c != '}' && c != '|') {
	    rpl_send(fd, ERR_ERRONEUSNICKNAME(param));
            return false;
        }
    }
    if (!server.checkNick(param))
    {
	rpl_send(fd, ERR_NICKNAMEINUSE(param));
	return false;
    }
    return true;
}

bool	isValidChannel(const std::string param, int fd , IRCServer& server)
{
    (void)server;
    if (param[0] == '0')
	return true;
    std::vector<std::string> channels = split(param, ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
	char firstChar = (*it)[0];
	if (firstChar != '#')
	{
	    rpl_send(fd, ERR_NOSUCHCHANNEL(*it));
	    return false;
	}
	for (size_t i = 1; i < (*it).length(); ++i) {
	    char c = (*it)[i];
	    if (!isalnum(c) && c != '-' && c != '[' && c != ']' &&
		c != '\\' && c != '^' && c != '_' && c != '{' &&
		c != '}' && c != '|') {
		rpl_send(fd, ERR_NOSUCHCHANNEL(*it));
		return false;
	    }
	}
    }
    return true;
}

bool	isConnected(const std::string param, int fd , IRCServer& server)
{
    (void) param;
    Client* client = (server.getClients()->find(fd))->second;
    if (client->GetLevel() >= CONNECTED)
    {
	rpl_send(fd, ERR_NOTREGISTERED());
	return (false);
    }
    return true;
}

bool	isAlphaNum(const std::string param, int fd , IRCServer& server)
{
    (void)server;
    for (std::string::const_iterator it2 = param.begin(); it2 != param.end(); it2++)
    {
	if (!isalnum((*it2)))
	{
	    rpl_send(fd, ERR_NOTREGISTERED());
	    return false;
	}
    }
    return true;
}