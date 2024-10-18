#include "NickCommand.hpp"
#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"

void NickCommand::execute(Client *client, std::map<std::string, std::vector<std::string>>& params)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    log(INFO, "_____nick command_____");
    std::string old_nick = client->GetNickname();
    client->SetNickname(params.find("nick")->second);
    client->SetPrefix();
    if (client->GetLevel() == REGISTERED)
    {
	std::string str = ":" + old_nick +"!"+ client->GetUsername() + "@" + client->GetHostname() + " NICK :" + client->GetNickname() +"\r\n";
	std::vector<Channel*> *channels = server.getChannels();
	for (std::vector<Channel*>::iterator it2 = channels->begin(); it2 != channels->end(); it2++)
	{
	    if ((*it2)->InChannel(client->GetNickname()))
	    {
		for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		    if (it->second != NULL && (*it2)->InChannel(it->second->GetNickname()))	
			send(it->first, str.c_str(), str.length(), 0);
		}
	    }

	}
    }
}
