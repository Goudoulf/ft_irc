#include "InviteCommand.hpp"
#include "IRCServer.hpp"

#include "debug.h"
#include "reply.h"

void InviteCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    log(CMD, client->getNickname() + ":_____invite_____");
    IRCServer *server = IRCServer::getInstance();
    std::string target = params.find("nickname")->second[0];
    std::string channelTargetName = params.find("channel")->second[0];
    Channel *channel = server->findChannel(channelTargetName);
    Client* clientTarget = server->findClient(target);
    std::string prefixSend = client->getPrefix();

    if (channel->inChannel(target))
    {
        rplSend(client->getSocket(), ERR_USERONCHANNEL(target, channelTargetName));
        return ;
    }
    rplSend(client->getSocket(), RPL_INVITING(client->getNickname(), target, channelTargetName));
    server->findChannel(channelTargetName)->addInvitation(clientTarget);
    rplSend(clientTarget->getSocket(), RPL_INVITED(channelTargetName, target, prefixSend));
}
