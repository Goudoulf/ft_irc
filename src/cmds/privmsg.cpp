#include "../../includes/cmds.h"
#include "../../includes/debug.h"
#include <vector>

void	privmsg(IRCServer &server, int fd, std::vector<std::string>& param)
{
	(void)param;
    Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____privmsg_____");
	std::string msg(":" + client->GetNickname() + "!" + client->GetUsername() + "@" 
						+ client->GetHostname() + " " + client->GetBuffer() + "\r\n");
	log(REPLY, ":" + client->GetNickname() + "!" + client->GetUsername() + "@" 
						+ client->GetHostname() + " " + client->GetBuffer() );
	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second && it->second->GetSocket() != client->GetSocket())
			send(it->second->GetSocket(), msg.c_str(), msg.length(), 0);
	}
}
