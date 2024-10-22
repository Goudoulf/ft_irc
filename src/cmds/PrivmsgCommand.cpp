#include "PrivmsgCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void PrivmsgCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    log(CMD, client->GetNickname() + ":_____privmsg_____");
    IRCServer *server = IRCServer::getInstance();
    std::string msg(":" + client->GetPrefix() + " PRIVMSG " + params.find("msgtarget")->second[0] + " :" + params.find("message")->second[0] + "\r\n");
    std::string target = params.find("msgtarget")->second[0];
    std::string message = params.find("message")->second[0];
    if (target.at(0) == '&' || target.at(0) == '+' || target.at(0) == '!' || target.at(0) == '#')
    {
	Channel *channel = server->find_channel(target);
	if (channel)
	    channel->sendReply(RPL_PRIVMSG(client->GetPrefix(),target, message));
    }
    log(REPLY, msg);
}
