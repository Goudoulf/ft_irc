#include "PartCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"


void PartCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	std::vector<std::string> channels = split(params.find("channel")->second, ',');

	// Process each channel and its corresponding key
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channel = channels[i];
		std::string message = "";

		// Handle the join operation for each channel and key
		if (params.find("trailing") != params.end())
			message = params.find("trailing")->second;
		partChannel(channel, message, client_fd, server);
	}
}

void	PartCommand::partChannel(std::string channel, std::string message, int fd, IRCServer &server)
{
    Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____part_____");
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
	{
		log(ERROR, "No channel :" + channel + "|");
		rpl_send(fd, ERR_NOSUCHCHANNEL(channel));
		return ;

	}
	if (!chan->InChannel(client->GetNickname()))
	{
		log(ERROR, client->GetNickname() + "not on channel " + channel);
		rpl_send(fd, ERR_NOTONCHANNEL(channel));
		return ;

	}
	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetNickname()))
			message_server(chan->getChannelName(), "PART", *client , ":" + message, it->first);
	}
	if (chan->InChannel(client->GetNickname()) == true)
		chan->remove_client(*client);
}
