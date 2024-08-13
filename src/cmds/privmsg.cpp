#include "../../includes/cmds.h"

void	privmsg(Client &client, IRCServer &server)
{
	std::cout << "_____privmsg_____" << std::endl;
	std::string msg(":" + client.GetNickname() + "!" + client.GetUsername() + "@" 
						+ client.GetHostname() + " " + client.buffer + "\r\n");
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if ((*_it)->GetSocket() != client.GetSocket())
			send((*_it)->GetSocket(), msg.c_str(), msg.length(), 0);
	}
}