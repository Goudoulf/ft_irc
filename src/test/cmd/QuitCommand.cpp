#include "../header/QuitCommand.hpp"
#include "../../../includes/cmds.h"
#include "../../../includes/debug.h"
#include "../../../includes/reply.h"

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
	server.remove_client(client);
	//if topic is set -> RPL_TOPIC
	// if (!chan->getTopic().empty())
	// 	reply_server("332", client, " " + chan->getChannelName() + " :" + chan->getTopic());
	// //RPL_NAMREPLY
	//
	// reply_server("353", client, "= " + channel + " :" + chan->getUsers());
	//
	// //RPL_ENDOFNAMES
	//
	// reply_server("366", client, channel + " :End of NAMES list");

}

void QuitCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	Client* client = (server.getClients()->find(client_fd))->second;
	for (std::vector<Channel*>::iterator _it = server.getChannels()->begin(); _it != server.getChannels()->end(); _it++) {
		if ((*_it)->InChannel(client->GetUsername()))
			quitServer2((*_it)->getChannelName() , params.find("message")->second, *client, server);
	}

}
