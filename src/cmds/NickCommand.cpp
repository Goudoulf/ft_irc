#include "NickCommand.hpp"
#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"

void NickCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    log(INFO, "_____nick command_____" +*params.find("nick")->second.begin() + "|");
    IRCServer *server = IRCServer::getInstance();
    std::string old_nick = client->getNickname();
    client->setNickname(*params.find("nick")->second.begin());
    client->setPrefix();
    if (client->getLevel() == REGISTERED)
    {
	std::string str = ":" + old_nick +"!"+ client->getUsername() + "@" + client->getHostname() + " NICK :" + client->getNickname() +"\r\n";
	std::vector<Channel*> *channels = server->getChannels();
	for (std::vector<Channel*>::iterator it2 = channels->begin(); it2 != channels->end(); it2++)
	{
	    if ((*it2)->inChannel(client->getNickname()))
	    {
		for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
		    if (it->second != NULL && (*it2)->inChannel(it->second->getNickname()))	
			send(it->first, str.c_str(), str.length(), 0);
		}
	    }

	}
    }
}
