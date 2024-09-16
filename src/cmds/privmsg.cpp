#include "../../includes/cmds.h"
#include "../../includes/debug.h"
#include <vector>

void	privmsg(IRCServer &server, int fd, std::vector<std::string>& param)
{
	log(CMD, client.GetNickname() + ":_____privmsg_____");
	std::string msg(":" + client.GetNickname() + "!" + client.GetUsername() + "@" 
						+ client.GetHostname() + " " + client.GetBuffer() + "\r\n");
	log(REPLY, ":" + client.GetNickname() + "!" + client.GetUsername() + "@" 
						+ client.GetHostname() + " " + client.GetBuffer() );
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if ((*_it)->GetSocket() != client.GetSocket())
			send((*_it)->GetSocket(), msg.c_str(), msg.length(), 0);
	}
}
