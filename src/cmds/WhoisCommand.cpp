#include "WhoisCommand.hpp"
#include <sstream>

void WhoisCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
    Client *target = server->findClient(params.find("target")->second[0]);
    std::string nickName = target->GetNickname();
    //rpl_send(target_fd, RPL_WHOISUSER(nickName, target->GetUsername(), target->GetHostname(), target->GetRealname()));
    std::vector<Channel *> *channels = server->getChannels();
    std::string channelsToSend;
    for (std::vector<Channel *>::iterator it = channels->begin(); it != channels->end(); it++)
    {
        if ((*it)->IsOp(nickName) && (*it)->InChannel(nickName))
            channelsToSend += "@";
        if ((*it)->InChannel(nickName))
            channelsToSend += (*it)->getChannelName() + " ";
    }
    //rpl_send(target_fd, RPL_WHOISCHANNELS(nickName, channelsToSend));
    std::stringstream ss, ss2;
    ss << time(NULL) - target->getLastActivity();
    ss2 << target->getSignOnTime();
    rpl_send(client->GetSocket(), RPL_WHOISUSER(client->GetNickname(), nickName, target->GetUsername(), target->GetHostname(), target->GetRealname()) 
                        + RPL_WHOISIDLE(client->GetNickname(), ss.str(), ss2.str())
                        + RPL_WHOISSERVER(client->GetNickname(), "NetTwerkers Deluxe Edition", "Insane Package Turbo Deluxe")
                        + RPL_WHOISCHANNELS(client->GetNickname(), channelsToSend)
                        + RPL_ENDOFWHOIS(client->GetNickname()));
   // rpl_send(target_fd, RPL_ENDOFWHOIS(nickName));
}
