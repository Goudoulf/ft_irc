#include "../../includes/cmds.h"
#include "../../includes/debug.h"
#include <algorithm>

// void	quit(Client &client, IRCServer &server)
// {
// 	std::cout << "_____quit_____" << std::endl;
// 	std::string buf = client.GetBufferString();
// 	std::string param (buf.substr(buf.find(":"), buf.find_first_of(" \r\n\0", 5) - 5));
// 	std::cout << "PARM =" << param << std::endl;
// 	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
// 			message_server((*_it)->GetNickname(), "QUIT", client, param, (*_it)->GetSocket());
// 	}
// 	(void)server;
// 	(void)client;
// }
//
void	quitServer(std::string channel, std::string message, Client &client, IRCServer &server)
{
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		log(ERROR, "No channel");
		// error no channel
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if (chan->InChannel((*_it)->GetUsername()))
			message_server("", "QUIT", client, message, (*_it)->GetSocket());
	}
	if (chan->InChannel(client.GetUsername()) == true)
		chan->remove_client(client);
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

void parseQuitCommand(const std::vector<std::string>& tokens, Client &client, IRCServer &server)
{
	// SVplit channels
	std::string message;

	// Split keys if provided

	if (tokens.size() > 1)
	{
		unsigned long i = 1;
		while (i < tokens.size())
		{
			if( i + 1 == tokens.size())
				message = message + tokens[i];
			else
				message = message + tokens[i] + " ";
			i++;
		}
	}
	// Handle the join operation for each channel and key
	
	for (std::vector<Channel*>::iterator _it = server.getChannels()->begin(); _it != server.getChannels()->end(); _it++) {
		if ((*_it)->InChannel(client.GetUsername()))
			quitServer((*_it)->getChannelName() , message, client, server);
	}
}

void	quit(IRCServer &server, int fd, std::vector<std::string>& params)
{
	log(CMD, client.GetNickname() + ":_____quit_____");
	std::string buf = client.GetBuffer();
	std::cout << buf << std::endl;
	buf = buf.substr(0, buf.find_first_of("\r\n\0", 5));
	std::vector<std::string> tokens = tokenize(buf);
	parseQuitCommand(tokens, client, server);
	server.remove_client(client);
}
