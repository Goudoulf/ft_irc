
#include "KickCommand.hpp"

void KickCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    
    IRCServer *server = IRCServer::getInstance();
    std::string msg = ":" + client->GetPrefix() + " KICK #test WOOHOO\r\n";
    send(server->findClient("WOOHOO")->GetSocket(), msg.c_str(), msg.length(), 0);
    // std::vector<std::string> channels = params.find("channel")->second;
    // std::vector<std::string> users = params.find("user")->second;
    // for (std::vector<std::string>::iterator channelsIt = channels.begin(); channelsIt != channels.end(); channelsIt++)
    // {
    //     Channel *channel = server->findChannel(*channelsIt);
    //     std::map<Client*, bool> channelUsers = channel->getUsersMap();
    //     for (std::vector<std::string>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++)
    //     {
    //         Client *clientToKick = server->findClient(*usersIt);
    //         if (clientToKick == NULL)
    //             return;

    //     }
    // }
    (void)client;
    (void)params;
}
