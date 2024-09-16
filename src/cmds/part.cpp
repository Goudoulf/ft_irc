//TODO: Create part command

#include "../../includes/cmds.h"
#include "../Channel.hpp"
#include <sstream>
#include <vector>
#include <string>
#include "../../includes/debug.h"

   // Command: PART
   // Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
   //
   // The PART command causes the user sending the message to be removed
   // from the list of active members for all given channels listed in the
   // parameter string.  If a "Part Message" is given, this will be sent
   // instead of the default message, the nickname.  This request is always
   // granted by the server.
   //
   // Servers MUST be able to parse arguments in the form of a list of
   // target, but SHOULD NOT use lists when sending PART messages to
   // clients.
   //
   // Numeric Replies:
   //
   //         ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
   //         ERR_NOTONCHANNEL
   //
   // Examples:
   //
   // PART #twilight_zone             ; Command to leave channel
   //                                 "#twilight_zone"
   //
   // PART #oz-ops,&group5            ; Command to leave both channels
   //                                 "&group5" and "#oz-ops".
   //
   // :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
   //                                 ; User WiZ leaving channel
   //                                 "#playzone" with the message "I
   //                                 lost".

void	partChannel(std::string channel, std::string message, Client &client, IRCServer &server)
{
	log(CMD, client.GetNickname() + ":_____part_____");
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		log(ERROR, "No channel");
		// error no channel
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if (chan->InChannel((*_it)->GetUsername()))
			message_server(chan->getChannelName(), "PART", client, message, (*_it)->GetSocket());
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

void parsePartCommand(const std::vector<std::string>& tokens, Client &client, IRCServer &server)
{
	// SVplit channels
	std::vector<std::string> channels = split(tokens[1], ',');
	std::string message;

	// Split keys if provided
	std::cout << tokens.size() << std::endl;
	if (tokens.size() > 2)
	{
		unsigned long i = 2;
		while (i < tokens.size())
		{
			if( i + 1 == tokens.size())
				message = message + tokens[i];
			else
				message = message + tokens[i] + " ";
			i++;
		}
	}

	// Process each channel and its corresponding key
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channel = channels[i];

		// Handle the join operation for each channel and key
		partChannel(channel, message, client, server);
	}
}

void	part(IRCServer &server, int fd, std::vector<std::string>& params)
{
	std::string buf = client.GetBuffer();
	std::cout << buf << std::endl;
	buf = buf.substr(0, buf.find_first_of("\r\n\0", 5));
	std::vector<std::string> tokens = tokenize(buf);
	parsePartCommand(tokens, client, server);
}
