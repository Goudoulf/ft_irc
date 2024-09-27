#include "PrivmsgCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void PrivmsgCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    log(CMD, client->GetNickname() + ":_____privmsg_____");
    std::string msg(":" + client->GetPrefix() + " PRIVMSG " + params.find("msgtarget")->second + " :" + params.find("message")->second + "\r\n");
    log(REPLY, msg);
    for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
	if (it->second && it->second->GetSocket() != client->GetSocket())
	    send(it->second->GetSocket(), msg.c_str(), msg.length(), 0);
    }
}
