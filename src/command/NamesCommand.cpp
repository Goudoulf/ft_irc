#include "NamesCommand.hpp"
#include "IRCServer.hpp"

#include "cmds.h"
#include "debug.h"
#include "reply.h"

void NamesCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	log(CMD, client->getNickname() + ":_____names_____");
	IRCServer *server = IRCServer::getInstance();
	Channel *channel = server->findChannel(params.find("channels")->second[0]);
	if (!channel)
	{
		client->replyServer(RPL_ENDOFNAMES(client->getNickname(), params.find("channels")->second[0]));
		return;
	}
	std::map<Client*, bool> clients = channel->getUsersMap();
	std::string clientsList;
	for (std::map<Client*, bool>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second)
			clientsList += "@";
		clientsList += (*it).first->getNickname() + " ";
	}
	std::string channelName = channel->getChannelName();
	std::string clientName = (server->getClients()->find(client->getSocket())->second)->getNickname();
	rplSend(client->getSocket(), RPL_NAMREPLY(clientName, channelName, clientsList)
				+ RPL_ENDOFNAMES(clientName, channel->getChannelName()));
}
