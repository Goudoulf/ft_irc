#include "../../includes/cmds.h"
#include <algorithm>

void	quit(Client &client, IRCServer &server)
{
	std::cout << "_____quit_____" << std::endl;
	std::string buf = client.GetBufferString();
	std::string param (buf.substr(buf.find(":"), buf.find_first_of(" \r\n\0", 5) - 5));
	std::cout << "PARM =" << param << std::endl;
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
			message_server((*_it)->GetNickname(), "QUIT", client, param, (*_it)->GetSocket());
	}
	(void)server;
	(void)client;
}

// void	partChannel(std::string channel, std::string message, Client &client, IRCServer &server)
// {
// 	log(CMD, client.GetNickname() + ":_____part_____");
// 	Channel *chan;
// 	if (!(chan = server.find_channel(channel)))
// 		log(ERROR, "No channel");
// 		// error no channel
// 	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
// 		if (chan->InChannel((*_it)->GetUsername()))
// 			message_server(chan->getChannelName(), "PART", client, message, (*_it)->GetSocket());
// 	}
// 	if (chan->InChannel(client.GetUsername()) == true)
// 		chan->remove_client(client);
// 	//if topic is set -> RPL_TOPIC
// 	// if (!chan->getTopic().empty())
// 	// 	reply_server("332", client, " " + chan->getChannelName() + " :" + chan->getTopic());
// 	// //RPL_NAMREPLY
// 	//
// 	// reply_server("353", client, "= " + channel + " :" + chan->getUsers());
// 	//
// 	// //RPL_ENDOFNAMES
// 	//
// 	// reply_server("366", client, channel + " :End of NAMES list");
//
// }
//
// void parsePartCommand(const std::vector<std::string>& tokens, Client &client, IRCServer &server)
// {
// 	// SVplit channels
// 	std::vector<std::string> channels = split(tokens[1], ',');
// 	std::string message;
//
// 	// Split keys if provided
// 	std::cout << tokens.size() << std::endl;
// 	if (tokens.size() > 2)
// 	{
// 		unsigned long i = 2;
// 		while (i < tokens.size())
// 		{
// 			if( i + 1 == tokens.size())
// 				message = message + tokens[i];
// 			else
// 				message = message + tokens[i] + " ";
// 			i++;
// 		}
// 	}
//
// 	// Process each channel and its corresponding key
// 	for (size_t i = 0; i < channels.size(); ++i) {
// 		std::string channel = channels[i];
//
// 		// Handle the join operation for each channel and key
// 		partChannel(channel, message, client, server);
// 	}
// }
//
// void	quit(Client &client, IRCServer &server)
// {
// 	std::string buf = client.GetBuffer();
// 	std::cout << buf << std::endl;
// 	buf = buf.substr(0, buf.find_first_of("\r\n\0", 5));
// 	std::vector<std::string> tokens = tokenize(buf);
// 	parsePartCommand(tokens, client, server);
// }
