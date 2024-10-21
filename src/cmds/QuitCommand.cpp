#include "QuitCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void	quitServer2(std::string channel, std::string message, Client *client, IRCServer *server)
{
	Channel *chan = server->find_channel(channel);
	for (std::map<int, Client*>::iterator it = server->getClients()->begin(); it != server->getClients()->end(); it++) {
		if (it->second && chan->InChannel(it->second->GetNickname()))
			message_server("", "QUIT", *client, message, it->second->GetSocket());
	}
	chan->remove_client(*client);
}

void QuitCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
    IRCServer *server = IRCServer::getInstance();
	for (std::vector<Channel*>::iterator _it = server->getChannels()->begin(); _it != server->getChannels()->end(); _it++) {
		if ((*_it)->InChannel(client->GetNickname()))
			quitServer2((*_it)->getChannelName() , params.find("message")->second[0], client, server);
	}
	server->remove_client(*client);
}
