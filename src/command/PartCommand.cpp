#include "PartCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void	PartCommand::partChannel(Channel *channel, std::string message, Client *client)
{
	IRCServer *server = IRCServer::getInstance();
	channel->sendReply(RPL_PART(client->getPrefix(), channel->getChannelName(), message));
	channel->removeClient(client);
	if (channel->getIsEmpty())
		server->removeChannel(channel);
}

void PartCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	log(CMD, client->getNickname() + ":_____part_____");
	IRCServer *server = IRCServer::getInstance();
	std::vector<std::string> channels = params.find("channel")->second;
	std::string message = (params.find("comment") != params.end()) ? params.find("comment")->second[0] : "";
	for (size_t i = 0; i < channels.size(); ++i)
	{
		Channel *channel = server->findChannel(channels[i]);
		partChannel(channel, message, client);
	}
}
