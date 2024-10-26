#include "NickCommand.hpp"
#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void NickCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    log(INFO, "_____nick command_____");
    IRCServer *server = IRCServer::getInstance();
    std::string old_nick = client->getNickname();
    std::string old_prefix = client->getPrefix();
    client->setNickname(*params.find("nick")->second.begin());
    client->setPrefix();
    if (client->getLevel() == REGISTERED)
    {
	std::map<std::string, Channel*> *channels = server->getChannels();
	for (std::map<std::string, Channel*>::iterator it2 = channels->begin(); it2 != channels->end(); it2++)
	{
	    if ((it2)->second->inChannel(client->getNickname()))
		it2->second->sendMessageWithSender(client, RPL_NEWNICK(old_prefix, client->getNickname()));
	}
    }
}
