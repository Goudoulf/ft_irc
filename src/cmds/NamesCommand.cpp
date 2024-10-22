#include "NamesCommand.hpp"
#include "cmds.h"
#include "debug.h"

void NamesCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	IRCServer *server = IRCServer::getInstance();
	Channel *channel = server->find_channel(params.find("channels")->second[0]);
	std::map<Client*, bool> clients = channel->getUsersMap();
	std::string clientsList;
	for (std::map<Client*, bool>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second)
			clientsList += "@";
		clientsList += (*it).first->GetNickname() + " ";
	}
	std::string channelName = "= " + channel->getChannelName();
	std::string clientName = (server->getClients()->find(client->GetSocket())->second)->GetNickname();
	rpl_send(client->GetSocket(), RPL_NAMREPLY(clientName, channelName, clientsList)
				+ RPL_ENDOFNAMES(clientName, channel->getChannelName()));
}
