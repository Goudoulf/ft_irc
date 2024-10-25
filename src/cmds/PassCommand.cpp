#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "PassCommand.hpp"

void PassCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	(void)params;
	log(INFO,"_____pass_____");
	client->setLevel(CONNECTED);
	return ;
}
