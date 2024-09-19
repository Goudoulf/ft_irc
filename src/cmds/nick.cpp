#include "../../includes/cmds.h"

void	nick(IRCServer &server, int fd, std::vector<std::string>& params)
{
	Client* client = (server.getClients()->find(fd))->second;
	std::cout << "_____nick command_____" << std::endl;
	client->SetNickname(params[0]);
	//set prefix
	std::cout << "New nick = " << client->GetNickname() << std::endl;
}
