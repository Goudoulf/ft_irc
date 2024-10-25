#include "JoinCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"
#include <map>
#include <string>

void	joinChannel2(std::string channel, std::string key, Client *client, IRCServer *server)
{

	log(CMD, client->getNickname() + ":_____join_____");
	log(CMD, channel + " key=" + key); 
	Channel *chan;
	if (!(chan = server->findChannel(channel)))
		chan = server->createChannel(channel, client, key);
	if (chan->inChannel(client->getNickname()) == false)
	{
		log(DEBUG, "Not in channel adding client"); 
		if (!chan->keyIsValid(key))
		{
			log(ERROR, "Wrong Channel Key " + channel);
			client->replyServer(ERR_BADCHANNELKEY(channel));
			return ;
		}
		chan->add_client(client);
	}
	// for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
	// 	if (it->second != NULL && chan->inChannel(it->second->getNickname()))
	// 		message_server("", "JOIN", *client, chan->getChannelName(), it->first);
	// }
	chan->sendReply(RPL_JOIN(client->getPrefix(), channel));
	if (!chan->getTopic().empty())
		reply_server("332", *client, " " + chan->getChannelName() + " :" + chan->getTopic());
	reply_server("353", *client, "= " + channel + " :" + chan->getUsers());
	reply_server("366", *client, channel + " :End of NAMES list");

}

void JoinCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	log(INFO, "cmd");
        IRCServer *server = IRCServer::getInstance();
	std::vector<std::string> channels = params.find("channel")->second;

	// Split keys if provided
	std::vector<std::string> keys;
	if (params.find("key") != params.end() && !params.find("key")->second.empty()) {
		keys = params.find("key")->second;
	}
	// Process each channel and its corresponding key
	size_t j = 0;
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *chan;
		std::string channel = channels[i];
		std::string key = "";
		if ((!server->findChannel(channel)) || ((chan = server->findChannel(channel)) && chan->getPassword().size()))
		{
			key = (j < keys.size()) ? keys[j] : "";
			j++;
		}
		joinChannel2(channel, key, client, server);
	}
}

