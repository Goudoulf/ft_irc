#include "JoinCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"
#include <map>
#include <string>

void	joinChannel2(std::string channel, std::string key, Client *client, IRCServer *server)
{

	log(DEBUG, channel + " key=" + key); 
	Channel *chan;
	if (!(chan = server->findChannel(channel)))
		chan = server->createChannel(channel, client, key);
	if (chan->inChannel(client->getNickname()) == false)
	{
		log(DEBUG, "Not in channel adding client"); 
		if (!chan->keyIsValid(key))
		{
			log(ERROR, "Wrong Channel Key " + channel);
			client->replyServer(ERR_BADCHANNELKEY(client->getNickname(), channel));
			return ;
		}
		chan->add_client(client);
	}
	chan->sendReply(RPL_JOIN(client->getPrefix(), channel));
	if (!chan->getTopic().empty())
		client->replyServer(RPL_TOPIC(channel, client->getNickname(), chan->getTopic()));
	client->replyServer(RPL_NAMREPLY(client->getNickname() , channel, chan->getUsers()));
	client->replyServer(RPL_ENDOFNAMES(client->getNickname(), channel));

}

void JoinCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	log(CMD, client->getNickname() + ":_____join_____");
        IRCServer *server = IRCServer::getInstance();
	std::vector<std::string> channels = params.find("channel")->second;

	std::vector<std::string> keys;
	if (params.find("key") != params.end() && !params.find("key")->second.empty()) {
		keys = params.find("key")->second;
	}

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

