
#include "../header/JoinCommand.hpp"

void	joinChannel(std::string channel, std::string key, int fd, IRCServer &server)
{

        Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____join_____");
	Channel *chan;
	if (!(chan = server.find_channel(channel)))
		chan = server.create_channel(channel, *client, key);
	if (chan->InChannel(client->GetUsername()) == false)
	{
		if (!chan->keyIsValid(key))
		{
			log(ERROR, "Wrong Channel Key");
			std::map<std::string, std::string> par {{"channel", channel}};
			sendIRCReply(*client, "475", par);
			return ;
		}
		chan->add_client(*client);
	}
	// add client send function
	for (std::map<int, Client*>::iterator it = server.getClients()->begin(); it != server.getClients()->end(); it++) {
		if (it->second != NULL && chan->InChannel(it->second->GetUsername()))
			message_server("", "JOIN", *client, chan->getChannelName(), it->first);
	}
	//if topic is set -> RPL_TOPIC
	if (!chan->getTopic().empty())
		reply_server("332", *client, " " + chan->getChannelName() + " :" + chan->getTopic());
	//RPL_NAMREPLY

	reply_server("353", *client, "= " + channel + " :" + chan->getUsers());

	//RPL_ENDOFNAMES

	reply_server("366", *client, channel + " :End of NAMES list");

}

void JoinCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    log(INFO, params[0]);
    std::vector<std::string> channels = split(params.find("channel")->second, ',');

    // Split keys if provided
    std::vector<std::string> keys;
    if (params.find("key")->second.empty()) {
	keys = split(params.find("key")->second, ',');
    }

    // Process each channel and its corresponding key
    for (size_t i = 0; i < channels.size(); ++i) {
	std::string channel = channels[i];
	std::string key = (i < keys.size()) ? keys[i] : "";

	// Handle the join operation for each channel and key
	joinChannel(channel, key, client_fd, server);
    }
}

