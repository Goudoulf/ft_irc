#include "PartCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void PartCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	std::vector<std::string> channels = split(params.find("channel")->second, ',');

	// Split keys if provided
	std::cout << params.size() << std::endl;

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
		// error no channel
		log(ERROR, "No channel :" + channel + "|");
		std::map<std::string, std::string> param {{"channel name", channel}};
		sendIRCReply(*client, "403", param );
		return ;

	}
	if (!chan->InChannel(client->GetUsername()))
	{
		// error no channel
		log(ERROR, client->GetUsername() + "not on channel " + channel);
		std::map<std::string, std::string> param {{"channel", channel}};
		sendIRCReply(*client, "442", param );
		return ;

	}

	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetUsername()))
			message_server(chan->getChannelName(), "PART", *client , ":" + message, it->first);
	}
	if (chan->InChannel(client->GetUsername()) == true)
		chan->remove_client(*client);
}
