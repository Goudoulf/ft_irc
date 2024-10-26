#include "PartCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void	PartCommand::partChannel(Channel *channel, std::string message, Client *client)
{
    IRCServer *server = IRCServer::getInstance();
	log(CMD, client->getNickname() + ":_____part_____");
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
		if (it->second != NULL && channel->inChannel(it->second->getNickname()))
			message_server(channel->getChannelName(), "PART", *client , ":" + message, it->first);
	}
	if (channel->inChannel(client->getNickname()) == true)
		channel->remove_client(client);
	if (channel->getIsEmpty())
		server->removeChannel(channel);
}

void PartCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	std::vector<std::string> channels = params.find("channel")->second;
	IRCServer *server = IRCServer::getInstance();

	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channelName = channels[i];
		Channel *channel = server->findChannel(channelName);
		std::string message = "";
		if (params.find("comment") != params.end())
			message = params.find("comment")->second[0];
		(*this).partChannel(channel, message, client);
	}
}
