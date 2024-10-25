#include "WhoisCommand.hpp"
#include <sstream>

void WhoisCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
    Client *target = server->findClient(params.find("target")->second[0]);
    std::string nickName = target->getNickname();
    //rpl_send(target_fd, RPL_WHOISUSER(nickName, target->getUsername(), target->getHostname(), target->getRealname()));
    std::vector<Channel *> *channels = server->getChannels();
    std::string channelsToSend;
    for (std::vector<Channel *>::iterator it = channels->begin(); it != channels->end(); it++)
    {
        if ((*it)->isOp(nickName) && (*it)->inChannel(nickName))
            channelsToSend += "@";
        if ((*it)->inChannel(nickName))
            channelsToSend += (*it)->getChannelName() + " ";
    }
    //rpl_send(target_fd, RPL_WHOISCHANNELS(nickName, channelsToSend));
    std::stringstream ss, ss2;
    ss << time(NULL) - target->getLastActivity();
    ss2 << target->getSignOnTime();
    rpl_send(client->getSocket(), RPL_WHOISUSER(client->getNickname(), nickName, target->getUsername(), target->getHostname(), target->getRealname()) 
                        + RPL_WHOISIDLE(client->getNickname(), ss.str(), ss2.str())
                        + RPL_WHOISSERVER(client->getNickname(), "NetTwerkers Deluxe Edition", "Insane Package Turbo Deluxe")
                        + RPL_WHOISCHANNELS(client->getNickname(), channelsToSend)
                        + RPL_ENDOFWHOIS(client->getNickname()));
   // rpl_send(target_fd, RPL_ENDOFWHOIS(nickName));
}
