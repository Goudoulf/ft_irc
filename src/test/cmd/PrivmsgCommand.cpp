#include "../header/PrivmsgCommand.hpp"
#include "../../../includes/cmds.h"
#include "../../../includes/debug.h"
#include "../../../includes/reply.h"

void PrivmsgCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    log(CMD, client->GetNickname() + ":_____privmsg_____");
    std::string msg(":" + client->GetNickname() + "!" + client->GetUsername() + "@" 
		    + client->GetHostname() + " " + params.find("message")->second + "\r\n");
    log(REPLY, ":" + client->GetNickname() + "!" + client->GetUsername() + "@" 
	+ client->GetHostname() + " " + client->GetBuffer() );
    for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
	if (it->second && it->second->GetSocket() != client->GetSocket())
	    send(it->second->GetSocket(), msg.c_str(), msg.length(), 0);
    }
}
