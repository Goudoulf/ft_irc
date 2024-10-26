#include "PrivmsgCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void PrivmsgCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    log(CMD, client->getNickname() + ":_____privmsg_____");
    IRCServer *server = IRCServer::getInstance();
    std::string target = params.find("msgtarget")->second[0];
    std::string message = params.find("message")->second[0];
    if (target.at(0) == '&' || target.at(0) == '+' || target.at(0) == '!' || target.at(0) == '#')
    {
	Channel *channel = server->findChannel(target);
	if (channel)
	    channel->sendMessage(client, RPL_PRIVMSG(client->getPrefix(),target, message));
	else
	    client->replyServer(ERR_NOSUCHCHANNEL(client->getNickname(), target));
    }
    else
    {
	Client * target_client;
	if ( (target_client = server->findClient(target)))
	   client->sendMessage(target_client->getSocket(), RPL_PRIVMSG(client->getPrefix(), target, message));
	else
	    client->replyServer(ERR_NOSUCHNICK(target));
    }
}
