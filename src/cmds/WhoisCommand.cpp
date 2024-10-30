#include "WhoisCommand.hpp"
#include <sstream>

void WhoisCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    IRCServer *server = IRCServer::getInstance();
    Client *target = server->findClient(params.find("target")->second[0]);
    std::string nickName = target->getNickname();
    //rplSend(target_fd, RPL_WHOISUSER(nickName, target->getUsername(), target->getHostname(), target->getRealname()));
    std::map<std::string, Channel *> *channels = server->getChannels();
    std::string channelsToSend;
    for (std::map<std::string, Channel *>::iterator it = channels->begin(); it != channels->end(); it++)
    {
        if (it->second->isOp(nickName) && it->second->inChannel(nickName))
            channelsToSend += "@";
        if (it->second->inChannel(nickName))
            channelsToSend += it->second->getChannelName() + " ";
    }
    //rplSend(target_fd, RPL_WHOISCHANNELS(nickName, channelsToSend));
    std::stringstream ss, ss2;
    ss << time(NULL) - target->getLastActivity();
    ss2 << target->getSignOnTime();
    rplSend(client->getSocket(), RPL_WHOISUSER(client->getNickname(), nickName, target->getUsername(), target->getHostname(), target->getRealname()) 
                        + RPL_WHOISIDLE(client->getNickname(), ss.str(), ss2.str())
                        + RPL_WHOISSERVER(client->getNickname(), "NetTwerkers Deluxe Edition", "Insane Package Turbo Deluxe")
                        + RPL_WHOISCHANNELS(client->getNickname(), channelsToSend)
                        + RPL_ENDOFWHOIS(client->getNickname()));
   // rplSend(target_fd, RPL_ENDOFWHOIS(nickName));
}
