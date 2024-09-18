#include "../../includes/cmds.h"
#include "../Channel.hpp"
#include <sstream>
#include <vector>
#include <string>
#include "../../includes/debug.h"

// Join message
//
//       Command: JOIN
//    Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//                / "0"
//
//    The JOIN command is used by a user to request to start listening to
//    the specific channel.  Servers MUST be able to parse arguments in the
//    form of a list of target, but SHOULD NOT use lists when sending JOIN
//    messages to clients.
//
//    Once a user has joined a channel, he receives information about
//    all commands his server receives affecting the channel.  This
//    includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
//    This allows channel members to keep track of the other channel
//    members, as well as channel modes.
//
//    If a JOIN is successful, the user receives a JOIN message as
//    confirmation and is then sent the channel's topic (using RPL_TOPIC) and
//    the list of users who are on the channel (using RPL_NAMREPLY), which
//    MUST include the user joining.
//
//    Note that this message accepts a special argument ("0"), which is
//    a special request to leave all channels the user is currently a member
//    of.  The server will process this message as if the user had sent
//    a PART command (See Section 3.2.2) for each channel he is a member
//    of.
//
//    Numeric Replies:
//
//            ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//            ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//            ERR_CHANNELISFULL               ERR_BADCHANMASK
//            ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//            ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
//            RPL_TOPIC
//
//    Examples:
//
//    JOIN #foobar                    ; Command to join channel #foobar.
//
//    JOIN &foo fubar                 ; Command to join channel &foo using
//                                    key "fubar".chan->getChannelName()chan->getChannelName()

void	joinChannel(std::string channel, std::string key, int fd, IRCServer &server)
{
	log(CMD, client.GetNickname() + ":_____join_____");
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		chan = server.create_channel(channel, client, key);
	if (chan->InChannel(client.GetUsername()) == false && chan->keyIsValid(key))
		chan->add_client(client);
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
		if (chan->InChannel((*_it)->GetUsername()))
			message_server("", "JOIN", client, chan->getChannelName(), (*_it)->GetSocket());
	}
	//if topic is set -> RPL_TOPIC
	if (!chan->getTopic().empty())
		reply_server("332", client, " " + chan->getChannelName() + " :" + chan->getTopic());
	//RPL_NAMREPLY

	reply_server("353", client, "= " + channel + " :" + chan->getUsers());

	//RPL_ENDOFNAMES

	reply_server("366", client, channel + " :End of NAMES list");

}

void parseJoinCommand(const std::vector<std::string>& tokens, int fd, IRCServer &server)
{
    // Split channels
    std::vector<std::string> channels = split(tokens[1], ',');

    // Split keys if provided
    std::vector<std::string> keys;
    if (tokens.size() > 2) {
        keys = split(tokens[2], ',');
    }

    // Process each channel and its corresponding key
    for (size_t i = 0; i < channels.size(); ++i) {
        std::string channel = channels[i];
        std::string key = (i < keys.size()) ? keys[i] : "";

        // Handle the join operation for each channel and key
        joinChannel(channel, key, fd, server);
    }
}

void	join(IRCServer &server, int fd, std::vector<std::string>& params)
{
	parseJoinCommand(params, fd, server);
}

