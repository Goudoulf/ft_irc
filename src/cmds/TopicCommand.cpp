#include "TopicCommand.hpp"
#include "reply.h"

void TopicCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
	std::string topic = "";
	if (params.size() > 1)
		topic = params.find("topic")->second[0];
	std::string channel = params.find("channel")->second[0];
	for (std::map<std::string, Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end(); _it++)
	{

		if ((_it)->second->getChannelName() == channel)
		{
			if (topic.empty())
			{
				if ((topic = _it->second->getTopic()).empty())
					rpl_send(client->getSocket(), RPL_NOTOPIC(channel));
				else
					rpl_send(client->getSocket(), RPL_TOPIC(channel, client->getNickname(), topic));
			}
			else
		{
				std::string nick = client->getNickname();
				if (_it->second->isOp(nick))
				{
					_it->second->setTopic(topic);
					std::string rpl = ":" + client->getPrefix() + " TOPIC " + _it->second->getChannelName() + " :" + _it->second->getTopic() + "\r\n";
					for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++)
					{
						if (it->second && _it->second->inChannel(it->second->getNickname()))
							send(it->second->getSocket(), rpl.c_str(), rpl.length(), 0);
					}
				}
				else
				rpl_send(client->getSocket(), ERR_CHANOPRIVSNEEDED(channel));
			}
		}
	}
}
