#include "QuitCommand.hpp"
#include "IRCServer.hpp"
#include "Channel.hpp"

#include "cmds.h"
#include "debug.h"

void	quitServer2(std::string channel, std::string message, Client *client, IRCServer *server)
{
	Channel *chan = server->findChannel(channel);
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
	{
		if (it->second && chan->inChannel(it->second->getNickname()))
			messageServer("", "QUIT", *client, message, it->second->getSocket());
	}
	chan->removeClient(client);
	if (chan->getIsEmpty())
		server->removeChannel(chan);
}

void QuitCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    IRCServer *server = IRCServer::getInstance();
	for (std::map<std::string, Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end();)
	{
		std::map<std::string, Channel *>::iterator _it2 = _it;
		_it2++;
		if (_it->second && _it->second->inChannel(client->getNickname()))
			quitServer2(_it->second->getChannelName() , params.find("message")->second[0], client, server);
		_it = _it2;
	}
	server->removeClient(client);
}

void QuitCommand::quitAll(Client *client, std::string message)
{
    IRCServer *server = IRCServer::getInstance();
	for (std::map<std::string, Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end();) {
		std::map<std::string, Channel *>::iterator _it2 = _it;
		_it2++;
		if (_it->second && _it->second->inChannel(client->getNickname()))
			quitServer2(_it->second->getChannelName() , message, client, server);
		_it = _it2;
	}
	server->removeClient(client);
}

void QuitCommand::quitAll2(Client *client, std::string message)
{
    IRCServer *server = IRCServer::getInstance();
	log(CMD, client->getNickname() + ":_____quit_____");
	for (std::map<std::string, Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end();)
	{
		std::map<std::string, Channel *>::iterator _it2 = _it;
		_it2++;
		if (_it->second && _it->second->inChannel(client->getNickname()))
			quitServer2(_it->second->getChannelName() , message, client, server);
		_it = _it2;
	}
}