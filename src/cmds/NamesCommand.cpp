
#include "NamesCommand.hpp"
#include "cmds.h"
#include "debug.h"

void NamesCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	Channel *channel = server.find_channel(params.find("channels")->second);
	std::map<Client, bool> clients = channel->getUsersMap();
	std::string clientsList;
	for (std::map<Client, bool>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second)
			clientsList += "@";
		clientsList += (*it).first.GetNickname() + " ";
	}
	std::string channelName = "= " + channel->getChannelName();
	std::string clientName = (server.getClients()->find(client_fd)->second)->GetNickname();
	rpl_send(client_fd, RPL_NAMREPLY(clientName, channelName, clientsList)
				+ RPL_ENDOFNAMES(clientName, channel->getChannelName()));
}
