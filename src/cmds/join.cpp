#include "../../includes/cmds.h"

void	join(Client &client, IRCServer &server)
{
	std::cout << "_____join_____" << std::endl;
	std::string buf = client.buffer;
	std::string chan_name (buf.substr(5, buf.find_first_of(" \r\n\0", 5) - 5));
	if (client.buffer[5] != '#')
	{
		std::string rpl(client.GetNickname() + " " + chan_name + " :No such channel\r\n");
		std::cout << rpl << std::endl;
		send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
	}
	else
	{
		//create channel if doesn't exist
		std::string test(":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetHostname() + " " + client.buffer + "\r\n");
		std::cout << client.buffer << std::endl;
		std::cout << test << std::endl;
		//send to all client on the channel (must be modified cause actually send to all client)
		for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
			if ((*_it)->GetSocket() != client.GetSocket())
				send(client.GetSocket(), test.c_str(), test.length(), 0);
		}
		//if topic is set -> RPL_TOPIC
		std::string rpl(":127.0.0.1 332 " + client.GetNickname() + chan_name + " :A timey-wimey channel\r\n");
		send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
		//RPL_NAMREPLY
		rpl = ":127.0.0.1 353 " + client.GetNickname() + " = " + chan_name + " :@ray cassie\r\n";
		send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
		//RPL_ENDOFNAMES
		rpl = ":127.0.0.1 366 " + client.GetNickname() + chan_name + " :End of NAMES list\r\n";
		send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
	}
	(void)server;
}