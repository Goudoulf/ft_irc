#include "TopicCommand.hpp"
#include "reply.h"

void TopicCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
	std::string topic = "";
	if (params.size() > 1)
		topic = params.find("topic")->second[0];
	std::string channel = params.find("channel")->second[0];
	for (std::vector<Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end(); _it++)
	{

		if ((*_it)->getChannelName() == channel)
		{
			if (topic.empty())
			{
				if ((topic = (*_it)->getTopic()).empty())
					rpl_send(client->GetSocket(), RPL_NOTOPIC(channel));
				else
					rpl_send(client->GetSocket(), RPL_TOPIC(channel, client->GetNickname(), topic));
			}
			else
			{
				std::string nick = client->GetNickname();
				if ((*_it)->IsOp(nick))
				{
					(*_it)->setTopic(topic);
					std::string rpl = ":" + client->GetPrefix() + " TOPIC " + (*_it)->getChannelName() + " :" + (*_it)->getTopic() + "\r\n";
					for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
					{
						if (it->second && (*_it)->InChannel(it->second->GetNickname()))
							send(it->second->GetSocket(), rpl.c_str(), rpl.length(), 0);
					}
				}
				else
					rpl_send(client->GetSocket(), ERR_CHANOPRIVSNEEDED(channel));
			}
		}
	}
}
