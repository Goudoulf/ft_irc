#include "cmds.h"
#include "debug.h"
#include "reply.h"
#include "PassCommand.hpp"

void PassCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	Client* client = (server.getClients()->find(client_fd))->second;
	log(INFO,"_____pass_____");
	if (params.size() < 1)
	{
		log(ERROR, "Wrong Number of param");
			return ;
	}
	client->SetIsConnected(true);
	return ;
}
