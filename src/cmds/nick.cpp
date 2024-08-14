#include "../../includes/cmds.h"

void	nick(Client &client, IRCServer &server)
{
	std::cout << "_____nick command_____" << std::endl;
	size_t index = client.GetBufferString().find("NICK");
	std::cout << "BUFFER = " << client.GetBufferString() << std::endl;
	std::string nick = client.GetBufferString().substr(index + 5, -1);
	client.SetNickname(nick.erase(nick.length() - 2, 2));
	std::cout << "New nick = " << client.GetNickname() << std::endl;
	std::string rpl(":127.0.0.1 001 " + client.GetNickname() + " :Welcome to the local Network " + client.GetNickname() +"\r\n");
	send(client.GetSocket(), rpl.c_str(), rpl.length(), 0);
	(void)server;
}