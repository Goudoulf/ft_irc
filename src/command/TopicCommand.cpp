#include "TopicCommand.hpp"
#include "IRCServer.hpp"

#include "reply.h"
#include "debug.h"

void TopicCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> > &params)
{
	log(CMD, client->getNickname() + ":_____topic_____");
	IRCServer *server = IRCServer::getInstance();
	std::string topic = "";
	if (params.size() > 1)
		topic = params.find("topic")->second[0];
	std::string channelName = params.find("channel")->second[0];
	Channel *channel = server->findChannel(channelName);
	if (topic.empty())
	{
		if ((topic = channel->getTopic()).empty())
			rplSend(client->getSocket(), RPL_NOTOPIC(client->getNickname(), channelName));
		else
			rplSend(client->getSocket(), RPL_TOPIC(channelName, client->getNickname(), topic));
		return;
	}
	if (channel->getIsTopicForOp())
	{
		if (!channel->isOp(client->getNickname()))
		{
			rplSend(client->getSocket(), ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getChannelName()));
			return;
		}
	}
	channel->setTopic(topic);
	std::string rpl = ":" + client->getPrefix() + " TOPIC " + channel->getChannelName() + " :" + channel->getTopic() + "\r\n";
	for (std::map<int, Client *>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
	{
		if (it->second && channel->inChannel(it->second->getNickname()))
			send(it->second->getSocket(), rpl.c_str(), rpl.length(), 0);
	}
}
