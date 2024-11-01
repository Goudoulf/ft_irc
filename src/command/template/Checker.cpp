#include "Channel.hpp"
#include "CmdLevel.h"
#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"
#include "cmds.h"

bool isValidInvite(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->findChannel(param);
    if (!channel)
        return (true);
    else
    {
        if (isOnChannel(param, client))
        {
            if (channel->getInviteOnly())
            {
                if (checkIsOp(param, client))
                    return (true);
            }
            else
                return (true);
        }
    }
    return (false);
}

bool	nickExists(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
	{
	    if (it->second && it->second->getNickname() == param)
		return true;
	}
	rplSend(client->getSocket(), ERR_NOSUCHNICK(param));
	return false;
}

bool	isOnChannel(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->findChannel(param);

    if (channel && !channel->inChannel(client->getNickname()))
    {
	    client->replyServer(ERR_NOTONCHANNEL(client->getNickname(), param));
        return false;
    }
    return true;
}

bool	channelExists(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    if (!server->findChannel(param))
    {
        rplSend(client->getSocket(), ERR_NOSUCHCHANNEL(client->getNickname(), param));
        return false;
    }
    return true;
}

bool	isValidPassword(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
	if (client->getLevel() >= CONNECTED)
	{
		client->replyServer(ERR_ALREADYREGISTRED(std::string("PASS")));
		return false;
	}
    if (server->getPassword() != param)
    {
        rplSend(client->getSocket(), ERR_PASSWDMISMATCH());
        return false;
    }
	return true;
}

bool	checkIsEmpty(const std::string param, Client *client)
{
    if (param.empty())
    {
	    rplSend(client->getSocket(), ERR_NONICKNAMEGIVEN());
        return false;
    }
    return true;
}

bool	isValidNick(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    if (param.length() > 9)
    {
	    rplSend(client->getSocket(), ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    char firstChar = param[0];
    if (!isalpha(firstChar) && firstChar != '-' && firstChar != '[' && firstChar != ']'
            && firstChar != '\\' && firstChar != '^' && firstChar != '_' && firstChar != '{'
            && firstChar != '}' && firstChar != '|')
    {
	    rplSend(client->getSocket(), ERR_ERRONEUSNICKNAME(param));
        return false;
    }
    for (size_t i = 1; i < param.length(); ++i)
    {
        char c = param[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']'
                && c != '\\' && c != '^' && c != '_' && c != '{'
                && c != '}' && c != '|')
        {
	        rplSend(client->getSocket(), ERR_ERRONEUSNICKNAME(param));
            return false;
        }
    }
    if (!server->checkNick(param))
    {
        rplSend(client->getSocket(), ERR_NICKNAMEINUSE(param));
        return false;
    }
    return true;
}

bool	isValidChannel(const std::string param, Client *client)
{
    if (param[0] == '0')
	    return true;
    std::vector<std::string> channels = split(param, ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        char firstChar = (*it)[0];
        if ((firstChar != '#' && firstChar != '!' && firstChar != '&' && firstChar != '+') || (*it).size() < 2)
        {
            rplSend(client->getSocket(), ERR_NOSUCHCHANNEL(client->getNickname(), *it));
            return false;
        }
        for (size_t i = 1; i < (*it).length(); ++i)
        {
            char c = (*it)[i];
            if (!isalnum(c) && c != '-' && c != '[' && c != ']'
                    && c != '\\' && c != '^' && c != '_' && c != '{'
                    && c != '}' && c != '|')
            {
                rplSend(client->getSocket(), ERR_NOSUCHCHANNEL(client->getNickname(), *it));
                return false;
            }
        }
    }
    return true;
}

bool    isInLimits(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->findChannel(param);
    if (!channel)
        return (true);
    if (channel->getIsLimited())
    {
        if (channel->getUsersMap().size() >= channel->getLimitSize())
        {
            rplSend(client->getSocket(), ERR_CHANNELISFULL(client->getNickname(), param));
            return (false);
        }
    }
    return (true);

}

bool   isInvited(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->findChannel(param);
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
        rplSend(client->getSocket(), ERR_INVITEONLYCHAN(client->getNickname(), channel->getChannelName()));
        return (false);
    }
    return (true);
}

bool    checkIsOp(const std::string param, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
    Channel *channel = server->findChannel(param);
    if (channel->isOp((client->getNickname())))
        return (true);
    rplSend(client->getSocket(), ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getChannelName()));
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
            rplSend(client->getSocket(), ERR_UNKNOWNMODE(client->getNickname(), modes.substr(i, 1)));
            return (false);
        }
    }
    return (true);
}

bool channelSupportsMode(const std::string param, Client *client)
{
    if (!param.empty() && param.at(0) == '+')
    {
        rplSend(client->getSocket(), ERR_NOCHANMODES(client->getNickname(), param));
        return (false);
    }
    return(true);
}
