// TODO: Create Topic command

#include "../../includes/cmds.h"
#include "../Channel.hpp"
#include "../../includes/debug.h"

void topic(IRCServer &server, int fd, std::vector<std::string>& params)
{
	std::string topic;
    Client* client = (server.getClients()->find(fd))->second;
	for (std::vector<Channel*>::iterator _it = server.getChannels()->begin(); _it != server.getChannels()->end(); _it++)
	{
		
		if ((*_it)->getChannelName().compare(params[1]) == 0)
		{
			if (topic.empty())
			{
				if ((*_it)->getTopic().empty())
					reply_server("331", *client, (*_it)->getChannelName() + " :No topic is set");
				else
					reply_server("332", *client, (*_it)->getChannelName() + " :" + (*_it)->getTopic());
			}
			else
			{
				if ((*_it)->IsOp(client->GetUsername()))
				{
					(*_it)->setTopic(topic);
					std::string rpl = ":" + client->GetPrefix() + " TOPIC " + (*_it)->getChannelName() + " :" + (*_it)->getTopic() + "\r\n";
					for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++)
					{
						if ((*_it)->InChannel(it->second->GetUsername()))
							send(it->second->GetSocket(), rpl.c_str(), rpl.length(), 0);
					}
				}
				else if ((*_it)->InChannel(client->GetUsername()))
					reply_server("482", *client, (*_it)->getChannelName() + " :You're not channel operator");
				else
					reply_server("442", *client, (*_it)->getChannelName() + " :You're not on that channel");
			}
		}
	}
}
