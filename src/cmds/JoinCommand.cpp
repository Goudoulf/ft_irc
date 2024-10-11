#include "JoinCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"
#include <map>
#include <string>

void	joinChannel2(std::string channel, std::string key, int fd, IRCServer &server)
{

        Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____join_____");
	log(CMD, channel + " key=" + key); 
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		chan = server.create_channel(channel, *client, key);
	if (chan->InChannel(client->GetUsername()) == false)
	{
		if (!chan->keyIsValid(key))
		{
			log(ERROR, "Wrong Channel Key");
			rpl_send(fd, ERR_BADCHANNELKEY(channel));
			return ;
		}
		chan->add_client(*client);
	}
	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetUsername()))
			message_server("", "JOIN", *client, chan->getChannelName(), it->first);
	}
	if (!chan->getTopic().empty())
		reply_server("332", *client, " " + chan->getChannelName() + " :" + chan->getTopic());
	reply_server("353", *client, "= " + channel + " :" + chan->getUsers());
	reply_server("366", *client, channel + " :End of NAMES list");

}

void JoinCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	log(INFO, "cmd");
	std::vector<std::string> channels = split(params.find("channel")->second, ',');

	// Split keys if provided
	std::vector<std::string> keys;
	if (params.find("key") != params.end() && !params.find("key")->second.empty()) {
		keys = split(params.find("key")->second, ',');
	}
	// Process each channel and its corresponding key
	size_t j = 0;
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *chan;
		std::string channel = channels[i];
		std::string key = "";
		if ((!server.find_channel(channel)) || ((chan = server.find_channel(channel)) && chan->getPassword().size()))
		{
			key = (j < keys.size()) ? keys[j] : "";
			j++;
		}
		joinChannel2(channel, key, client_fd, server);
	}
}

