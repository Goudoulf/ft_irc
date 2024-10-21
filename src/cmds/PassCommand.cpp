#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "PassCommand.hpp"

void PassCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	(void)params;
	Client* client = (server.getClients()->find(client_fd))->second;
	log(INFO,"_____pass_____");
	client->SetLevel(CONNECTED);
	return ;
}
