#include "WhoisCommand.hpp"
#include <sstream>
void WhoisCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = server.findClient(params.find("target")->second);
    std::string nickName = client->GetNickname();
    //rpl_send(client_fd, RPL_WHOISUSER(nickName, client->GetUsername(), client->GetHostname(), client->GetRealname()));
    std::vector<Channel *> *channels = server.getChannels();
    std::string channelsToSend;
    for (std::vector<Channel *>::iterator it = channels->begin(); it != channels->end(); it++)
    {
        if ((*it)->IsOp(nickName) && (*it)->InChannel(nickName))
            channelsToSend += "@";
        if ((*it)->InChannel(nickName))
            channelsToSend += (*it)->getChannelName() + " ";
    }
    //rpl_send(client_fd, RPL_WHOISCHANNELS(nickName, channelsToSend));
    std::stringstream ss, ss2;
    ss << time(NULL) - client->getLastActivity();
    ss2 << client->getSignOnTime();
    rpl_send(client_fd, RPL_WHOISUSER(nickName, nickName, client->GetUsername(), client->GetHostname(), client->GetRealname()) 
                        + RPL_WHOISIDLE(nickName, ss.str(), ss2.str())
                        + RPL_WHOISSERVER(nickName, "NetTwerkers Deluxe Edition", "Insane Package Turbo Deluxe")
                        + RPL_WHOISCHANNELS(nickName, channelsToSend)
                        + RPL_ENDOFWHOIS(nickName));
   // rpl_send(client_fd, RPL_ENDOFWHOIS(nickName));
}
