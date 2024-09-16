// TODO: Create Topic command

#include "../../includes/cmds.h"
#include "../Channel.hpp"
#include "../../includes/debug.h"

void topic(IRCServer &server, int fd, std::vector<std::string>& params)
{
	std::string buff(client.GetBufferString());
	std::string target(buff.substr(6, buff.find_first_of(" \r", 6) - 6));
	std::string topic;
	std::string::size_type pos = buff.find(":", buff.find_first_of(" \r", 6));
	if (pos != std::string::npos)
		topic = buff.substr(pos + 1, buff.find("\r") - pos - 1);
	for (std::vector<Channel*>::iterator _it = server.getChannels()->begin(); _it != server.getChannels()->end(); _it++)
	{
		
		if ((*_it)->getChannelName().compare(target) == 0)
		{
			if (topic.empty())
			{
				if ((*_it)->getTopic().empty())
					reply_server("331", client, (*_it)->getChannelName() + " :No topic is set");
				else
					reply_server("332", client, (*_it)->getChannelName() + " :" + (*_it)->getTopic());
			}
			else
			{
				if ((*_it)->IsOp(client.GetUsername()))
				{
					(*_it)->setTopic(topic);
					std::string rpl = ":" + client.GetPrefix() + " TOPIC " + (*_it)->getChannelName() + " :" + (*_it)->getTopic() + "\r\n";;
					for (std::vector<Client*>::iterator _it_client = server.getClients()->begin(); _it_client != server.getClients()->end(); _it_client++)
					{
						if ((*_it)->InChannel((*_it_client)->GetUsername()))
							send((*_it_client)->GetSocket(), rpl.c_str(), rpl.length(), 0);
					}
				}
				else if ((*_it)->InChannel(client.GetUsername()))
					reply_server("482", client, (*_it)->getChannelName() + " :You're not channel operator");
				else
					reply_server("442", client, (*_it)->getChannelName() + " :You're not on that channel");
			}
		}
	}
}
