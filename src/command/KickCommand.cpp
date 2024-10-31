
#include "KickCommand.hpp"
#include "IRCServer.hpp"
#include "PartCommand.hpp"

#include "debug.h"
#include "reply.h"

void KickCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    log(CMD, client->getNickname() + ":_____kick_____");
    IRCServer *server = IRCServer::getInstance();
    std::string channelName = params.find("channel")->second[0];
    std::vector<std::string> users = params.find("user")->second;
    Channel *channel = server->findChannel(channelName);
    for (std::vector<std::string>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++)
    {
        Client *clientToKick = server->findClient(*usersIt);
        if (clientToKick == NULL || !channel->inChannel(clientToKick->getNickname()))
        {
            rplSend(client->getSocket(), ERR_USERNOTINCHANNEL((*usersIt), channelName));
            return;
        }
        std::string message = "";
		if (params.find("comment") != params.end())
	        message = params.find("comment")->second[0];
        PartCommand::partChannel(channel, message, clientToKick);
    }
}
