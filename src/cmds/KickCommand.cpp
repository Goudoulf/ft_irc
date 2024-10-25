
#include "KickCommand.hpp"
#include "PartCommand.hpp"

void KickCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    
    IRCServer *server = IRCServer::getInstance();
    // std::string msg = ":" + client->getPrefix() + " KICK #test WOOHOO\r\n";
    // send(server->findClient("WOOHOO")->getSocket(), msg.c_str(), msg.length(), 0);
    std::vector<std::string> channels = params.find("channel")->second;
    std::vector<std::string> users = params.find("user")->second;
    for (std::vector<std::string>::iterator channelsIt = channels.begin(); channelsIt != channels.end(); channelsIt++)
    {
        Channel *channel = server->findChannel(*channelsIt);
        for (std::vector<std::string>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++)
        {
            Client *clientToKick = server->findClient(*usersIt);
            std::cout << *usersIt << std::endl;
            if (clientToKick == NULL || !channel->inChannel(clientToKick->getNickname()))
            {
                rpl_send(client->getSocket(), ERR_USERNOTINCHANNEL((*usersIt), channel->getChannelName()));
                return;
            }
            std::string message = "";
		    if (params.find("comment") != params.end())
			    message = params.find("comment")->second[0];
            PartCommand::partChannel(channel, message, clientToKick);
        }
    }
}
