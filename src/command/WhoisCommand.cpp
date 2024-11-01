#include "WhoisCommand.hpp"
#include "IRCServer.hpp"

#include "reply.h"

#include <sstream>

void WhoisCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    IRCServer *server = IRCServer::getInstance();
    Client *target = server->findClient(params.find("target")->second[0]);
    if (!target)
    {
        client->replyServer(ERR_NOSUCHNICK(client->getNickname(),params.find("target")->second[0] ));
        return ;
    }
    std::string nickName = target->getNickname();
    std::map<std::string, Channel *> *channels = server->getChannels();
    std::string channelsToSend;
    for (std::map<std::string, Channel *>::iterator it = channels->begin(); it != channels->end(); it++)
    {
        if (it->second->isOp(nickName) && it->second->inChannel(nickName))
            channelsToSend += "@";
        if (it->second->inChannel(nickName))
            channelsToSend += it->second->getChannelName() + " ";
    }
    std::stringstream ss, ss2;
    ss << time(NULL) - target->getLastActivity();
    ss2 << target->getSignOnTime();
    rplSend(client->getSocket(), RPL_WHOISUSER(client->getNickname(), nickName, target->getUsername(), target->getHostname(), target->getRealname()) 
                        + RPL_WHOISIDLE(client->getNickname(), ss.str(), ss2.str())
                        + RPL_WHOISSERVER(client->getNickname(), "NetTwerkers Deluxe Edition", "Insane Package Turbo Deluxe")
                        + RPL_WHOISCHANNELS(client->getNickname(), channelsToSend)
                        + RPL_ENDOFWHOIS(client->getNickname()));
}
