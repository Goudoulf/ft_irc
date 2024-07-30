#include "../../includes/cmds.h"

void	quit(Client &client, IRCServer &server)
{
	std::cout << "quit" << std::endl;
	(void)server;
	(void)client;
}