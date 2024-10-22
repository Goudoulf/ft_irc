#include "Channel.hpp"
#include "CmdLevel.h"
#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"

bool isValidInvite(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (!channel)
        return (true);
    else
    {
        if (isOnChannel(param, client))
        {
            if (channel->getInviteOnly())
            {
                if (isOp(param, client))
                    return (true);
            }
            else
                return (true);
        }
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
    {
        rpl_send(client->GetSocket(), ERR_PASSWDMISMATCH());
        return false;
    }
	return true;
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
	if (firstChar != '#' && firstChar != '!' && firstChar != '&' && firstChar != '+')
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

bool    isInLimits(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (!channel)
        return (true);
    if (channel->getIsLimited())
        if (channel->getUsersMap().size() >= channel->getLimitSize())
        {
            rpl_send(client->GetSocket(), ERR_CHANNELISFULL(param));
            return (false);
        }
    return (true);

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

bool    isTmodeOn(const std::string param, Client *client) //ONLY FOR SETUP
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (channel->getIsTopicForOp())
    {
        if (isOp(param, client))
            return (false);
    }
    return (true);
}

bool   isInvited(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (!channel)
        return (true);
    if (channel->getInviteOnly())
    {
        std::vector<Client *> invitationList = channel->getInvitationList();
        for(std::vector<Client *>::iterator it = invitationList.begin(); it != invitationList.end(); it++)
        {
            if ((*it) == client)
                return (true);
        }
        rpl_send(client->GetSocket(), ERR_INVITEONLYCHAN(channel->getChannelName()));
        return (false);
    }
    return (true);
}

bool    isOp(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->find_channel(param);
    if (channel->IsOp((server->getClients()->find(client->GetSocket()))->second->GetNickname()))
        return (true);
    rpl_send(client->GetSocket(), ERR_CHANOPRIVSNEEDED(param));
    return(false);
}

bool    isValidMode(const std::string param, Client *client)
{
    std::vector<std::string> modesSplit = split(param, ' ');
    std::string modes = modesSplit.front();
    std::string validModes = "itkol";
    for (unsigned int i = 1; i < modes.length(); i++)
    {
        if (validModes.find(modes.at(i)) > 5)
        {
            rpl_send(client->GetSocket(), ERR_UNKNOWNMODE(modes.substr(i, 1)));
            return (false);
        }
    }
    return (true);
}
