#include "QuitCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void	quitServer2(std::string channel, std::string message, Client &client, IRCServer &server)
{
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		log(ERROR, "No channel");
		// error no channel
	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second && chan->InChannel(it->second->GetUsername()))
			message_server("", "QUIT", client, message, it->second->GetSocket());
	}
	if (chan->InChannel(client.GetUsername()) == true)
		chan->remove_client(client);
}

void QuitCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	Client* client = (server.getClients()->find(client_fd))->second;
	for (std::vector<Channel*>::iterator _it = server.getChannels()->begin(); _it != server.getChannels()->end(); _it++) {
		if ((*_it)->InChannel(client->GetUsername()))
			quitServer2((*_it)->getChannelName() , params.find("message")->second, *client, server);
	}
	server.remove_client(*client);
}
