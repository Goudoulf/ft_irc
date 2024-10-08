#include "PongCommand.hpp"
#include "cmds.h"
#include "debug.h"

void PongCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	(void)params;
    Client* client = (server.getClients()->find(client_fd))->second;
	log(CMD, client->GetNickname() + ":_____pong_____");
}
