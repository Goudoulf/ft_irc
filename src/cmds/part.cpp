//TODO: Create part command

#include "../../includes/cmds.h"
#include "../../includes/reply.h"
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

void	partChannel(std::string channel, std::string message, int fd, IRCServer &server)
{
        Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____part_____");
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
	{
		// error no channel
		log(ERROR, "No channel :" + channel + "|");
		std::map<std::string, std::string> param {{"channel name", channel}};
		sendIRCReply(*client, "403", param );
		return ;

	}
	if (!chan->InChannel(client->GetUsername()))
	{
		// error no channel
		log(ERROR, client->GetUsername() + "not on channel " + channel);
		std::map<std::string, std::string> param {{"channel", channel}};
		sendIRCReply(*client, "442", param );
		return ;

	}

	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetUsername()))
			message_server(chan->getChannelName(), "PART", *client , ":" + message, it->first);
	}
	if (chan->InChannel(client->GetUsername()) == true)
		chan->remove_client(*client);
}

void parsePartCommand(const std::vector<std::string>& tokens, int fd, IRCServer &server)
{
	// SVplit channels
	std::vector<std::string> channels = split(tokens[0], ',');

	// Split keys if provided
	std::cout << tokens.size() << std::endl;

	// Process each channel and its corresponding key
	for (size_t i = 0; i < channels.size(); ++i) {
		std::string channel = channels[i];

		// Handle the join operation for each channel and key
		partChannel(channel, tokens[1], fd, server);
	}
}

void	part(IRCServer &server, int fd, std::vector<std::string>& params)
{
	parsePartCommand(params, fd, server);
}
