#include "PrivmsgCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void PrivmsgCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    log(CMD, client->GetNickname() + ":_____privmsg_____");
    IRCServer *server = IRCServer::getInstance();
    std::string msg(":" + client->GetPrefix() + " PRIVMSG " + params.find("msgtarget")->second[0] + " :" + params.find("message")->second[0] + "\r\n");
    log(REPLY, msg);
    for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
        if (it->second && it->second->GetSocket() != client->GetSocket())
            send(it->second->GetSocket(), msg.c_str(), msg.length(), 0);
    }
}
