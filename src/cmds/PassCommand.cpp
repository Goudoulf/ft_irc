#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "PassCommand.hpp"

void PassCommand::execute(Client *client, std::map<std::string, std::vector<std::string>>& params)
{
	(void)params;
	Client* client = (server.getClients()->find(client_fd))->second;
	log(INFO,"_____pass_____");
	client->SetLevel(CONNECTED);
	return ;
}
