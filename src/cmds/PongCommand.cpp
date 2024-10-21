#include "PongCommand.hpp"
#include "cmds.h"
#include "debug.h"

void PongCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	(void)params;
	log(CMD, client->GetNickname() + ":_____pong_____");
}
