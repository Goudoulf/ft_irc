#include "PongCommand.hpp"
#include "cmds.h"
#include "debug.h"

void PongCommand::execute(Client *client, std::map<std::string, std::vector<std::string>>& params)
{
	(void)params;
    Client* client = (server.getClients()->find(client_fd))->second;
	log(CMD, client->GetNickname() + ":_____pong_____");
}
