#include "PartCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void	partChannel(std::string channel, std::string message, Client *client)
{
        IRCServer *server = IRCServer::getInstance();
	log(CMD, client->GetNickname() + ":_____part_____");
	Channel *chan;
	if (!(chan = server->findChannel(channel)))
	{
		log(ERROR, "No channel :" + channel + "|");
		rpl_send(client->GetSocket(), ERR_NOSUCHCHANNEL(client->GetNickname(), channel));
		return ;

	}
	if (!chan->InChannel(client->GetNickname()))
	{
		log(ERROR, client->GetNickname() + "not on channel " + channel);
		rpl_send(client->GetSocket(), ERR_NOTONCHANNEL(channel));
		return ;

	}
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetNickname()))
			message_server(chan->getChannelName(), "PART", *client , ":" + message, it->first);
	}
	if (chan->InChannel(client->GetNickname()) == true)
		chan->remove_client(client);
	if (chan->getIsEmpty())
		server->removeChannel(chan);
}

void PartCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	std::vector<std::string> channels = params.find("channel")->second;

	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channel = channels[i];
		std::string message = "";

		// Handle the join operation for each channel and key
		if (params.find("trailing") != params.end())
			message = params.find("trailing")->second[0];
		partChannel(channel, message, client);
	}
}
