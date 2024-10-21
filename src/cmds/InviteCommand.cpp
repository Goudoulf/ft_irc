#include "InviteCommand.hpp"

void InviteCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
    std::string target = params.find("nickname")->second[0];
    std::string channelTarget = params.find("channel")->second[0];
    Client* clientTarget = server->findClient(target);
    std::string prefixSend = server->getClients()->find(client->GetSocket())->second->GetPrefix();

    rpl_send(client->GetSocket(), RPL_INVITING(channelTarget, target));
    server->find_channel(channelTarget)->addInvitation(*clientTarget);
    rpl_send(clientTarget->GetSocket(), RPL_INVITED(channelTarget, target, prefixSend));
}
