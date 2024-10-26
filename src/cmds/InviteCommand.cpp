#include "InviteCommand.hpp"

void InviteCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
    std::string target = params.find("nickname")->second[0];
    std::string channelTargetName = params.find("channel")->second[0];
    Channel *channel = server->findChannel(channelTargetName);
    Client* clientTarget = server->findClient(target);
    std::string prefixSend = client->getPrefix();

    if (channel->inChannel(target))
    {
        rpl_send(client->getSocket(), ERR_USERONCHANNEL(target, channelTargetName));
        return ;
    }
    rpl_send(client->getSocket(), RPL_INVITING(client->getNickname(), target, channelTargetName));
    server->findChannel(channelTargetName)->addInvitation(clientTarget);
    rpl_send(clientTarget->getSocket(), RPL_INVITED(channelTargetName, target, prefixSend));
}
