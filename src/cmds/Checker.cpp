#include "Channel.hpp"
#include "CmdLevel.h"
#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"


/**
 * if chan not exist
 * {
 *  send invite
 * }
 * !!!
 * {
 *  if client is on chan
 *  {
 *     if +i enabled
 *      {
 *          if client is op on chan
 *          {
 *              send invite
 *          }
 *          !!!
 *          {
 *              ERR_CHANOPRIVSNEEDED
 *          }
 *      }
 *      !!!
 *      {
 *          send invite
 *      }
 *  }
 *  !!!
 *  {
 *     ERR_NOTONCHANNEL
 *  }
 * }
 */
bool    isValidInvite(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (!channel)
        return (true);
    else
    {
        if (isOnChannel(param, client))
        {
            if (channel->getIsInviteForOp())
            {
                if (channel->IsOp((server->getClients()->find(client->GetSocket()))->second->GetNickname()))
                    return (true);
                else
                    rpl_send(client->GetSocket(), ERR_CHANOPRIVSNEEDED(param));
            }
            else
                return (true);
        }
        else
            rpl_send(client->GetSocket(), ERR_NOTONCHANNEL(param));

    }
    return (false);
}

bool	nickExist(const std::string param, Client *client)
{
        IRCServer *server = IRCServer::getInstance();
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
	{
	    if (it->second && it->second->GetNickname() == param)
		return true;
	}
	rpl_send(client->GetSocket(), ERR_NOSUCHNICK(param));
	return false;
}

bool	isOnChannel(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);

    if (channel && !channel->InChannel(client->GetNickname()))
    {
	rpl_send(client->GetSocket(), ERR_NOTONCHANNEL(param));
	return false;
    }
    return true;
}

bool	ChannelExist(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    if (!server->find_channel(param))
    {
	rpl_send(client->GetSocket(), ERR_NOSUCHCHANNEL(param));
	return false;
    }
    return true;
}

bool	isValidPassword(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    if (server->getPassword() == param)
	return true;
    rpl_send(client->GetSocket(), ERR_PASSWDMISMATCH());
    return false;
}

bool	isEmpty(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    (void)server;
    if (param.empty())
    {
	rpl_send(client->GetSocket(), ERR_NONICKNAMEGIVEN());
        return false;
    }
    return true;
}

bool	isValidNick(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    if (param.length() > 9)
    {
	rpl_send(client->GetSocket(), ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    char firstChar = param[0];
    if (!isalpha(firstChar) && firstChar != '-' && firstChar != '[' && firstChar != ']' &&
        firstChar != '\\' && firstChar != '^' && firstChar != '_' && firstChar != '{' &&
        firstChar != '}' && firstChar != '|') {
	rpl_send(client->GetSocket(), ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    for (size_t i = 1; i < param.length(); ++i) {
        char c = param[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']' &&
            c != '\\' && c != '^' && c != '_' && c != '{' &&
            c != '}' && c != '|') {
	    rpl_send(client->GetSocket(), ERR_ERRONEUSNICKNAME(param));
            return false;
        }
    }
    if (!server->checkNick(param))
    {
	rpl_send(client->GetSocket(), ERR_NICKNAMEINUSE(param));
	return false;
    }
    return true;
}

bool	isValidChannel(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    (void)server;
    if (param[0] == '0')
	return true;
    std::vector<std::string> channels = split(param, ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
	char firstChar = (*it)[0];
	if (firstChar != '#')
	{
	    rpl_send(client->GetSocket(), ERR_NOSUCHCHANNEL(*it));
	    return false;
	}
	for (size_t i = 1; i < (*it).length(); ++i) {
	    char c = (*it)[i];
	    if (!isalnum(c) && c != '-' && c != '[' && c != ']' &&
		c != '\\' && c != '^' && c != '_' && c != '{' &&
		c != '}' && c != '|') {
		rpl_send(client->GetSocket(), ERR_NOSUCHCHANNEL(*it));
		return false;
	    }
	}
    }
    return true;
}

bool	isConnected(const std::string param, Client *client)
{
    (void) param;
    if (client->GetLevel() >= CONNECTED)
    {
	rpl_send(client->GetSocket(), ERR_NOTREGISTERED());
	return (false);
    }
    return true;
}

bool	isAlphaNum(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    (void)server;
    for (std::string::const_iterator it2 = param.begin(); it2 != param.end(); it2++)
    {
	if (!isalnum((*it2)))
	{
	    rpl_send(client->GetSocket(), ERR_NOTREGISTERED());
	    return false;
	}
    }
    return true;
}
