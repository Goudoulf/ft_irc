#include "../../includes/cmds.h"

void	nick(IRCServer &server, int fd, std::vector<std::string>& params)
{
    Client* client = (server.getClients()->find(fd))->second;
	std::cout << "_____nick command_____" << std::endl;
	client->SetNickname(params[1]);
	//set prefix
	std::cout << "New nick = " << client->GetNickname() << std::endl;
	std::string rpl(":127.0.0.1 001 " + client->GetNickname() + " :Welcome to the local Network " + client->GetNickname() +"\r\n");
	send(client->GetSocket(), rpl.c_str(), rpl.length(), 0);
}
