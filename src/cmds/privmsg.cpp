#include "../../includes/cmds.h"

void	privmsg(Client &client, IRCServer &server)
{
	std::cout << "privmsg" << std::endl;
	std::string test2(":" + client.GetNickname() + "!" + client.GetUsername() + "@" 
						+ client.GetHostname() + " " + client.buffer + "\r\n");
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if ((*_it)->GetSocket() != client.GetSocket())
		{
			std::cout << test2 << std::endl;
			send((*_it)->GetSocket(), test2.c_str(), test2.length(), 0);
		}
	}
}