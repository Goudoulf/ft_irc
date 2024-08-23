#include "../../includes/cmds.h"
#include "../Channel.hpp"

void	join(Client &client, IRCServer &server)
{
	Channel *channel = NULL;
	std::cout << "_____join_____" << std::endl;
	std::string buf = client.GetBufferString();
	std::string chan_name (buf.substr(5, buf.find_first_of(" \r\n\0", 5) - 5));
	std::cout << buf << std::endl;
	if (client.GetBuffer()[5] != '#')
	{
		std::string rpl(client.GetNickname() + " " + chan_name + " :No such channel\r\n");
		std::cout << rpl << std::endl;
		send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
	}
	else
	{
		//create channel if doesn't exist
		if (!(channel = server.find_channel(chan_name)))
			channel = server.create_channel(chan_name, client);
		std::string test(":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetHostname() + " " + buf + "\r\n");
		std::cout << buf << std::endl;
		std::cout << "TEST =" << test << std::endl;
		// TODO: Create class Channel and add client to it

		if (channel->InChannel(client.GetUsername()) == false)
			channel->add_client(client);

		//TODO: send to all client on the channel (must be modified cause actually send to all client)
		for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
			if (channel->InChannel((*_it)->GetUsername()))
				message_server("", "JOIN", **_it, channel->getChannelName());
				//send((*_it)->GetSocket(), test.c_str(), test.length(), 0);
		}
		//if topic is set -> RPL_TOPIC
		// if (!channel->getTopic().empty())
		// {
			// std::string rpl(":127.0.0.1 332 " + client.GetNickname() + chan_name + " :A timey-wimey channel\r\n");
			// send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
		reply_server("332", client, " " + channel->getChannelName() + " :" + channel->getTopic());
		//}
		//RPL_NAMREPLY
		reply_server("353", client, "= " + chan_name + " :" + channel->getUsers());
		// std::string rpl = ":127.0.0.1 353 " + client.GetNickname() + " = " + chan_name + " :@ray cassie\r\n";
		// send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
		
		//RPL_ENDOFNAMES

		reply_server("366", client, chan_name + " :End of NAMES list");

		//std::string rpl = ":127.0.0.1 366 " + "goudoulf" + " #tutu :End of NAMES list\r\n";
		// rpl = ":127.0.0.1 366 " + client.GetNickname() + chan_name + " :End of NAMES list\r\n";
		// send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
		// std::cout << "len =" << rpl.length() << std::endl;
		// std::cout << rpl << std::endl;

		//RPL_NAMEREPLY and RPL_ENDOFNAMES maybe not send on the right channel (actually send on the main chat not the channel)
	}
}
