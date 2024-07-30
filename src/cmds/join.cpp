#include "../../includes/cmds.h"

void	join(Client &client, IRCServer &server)
{
	std::cout << "join" << std::endl;
	int pos;
	for (int i = 4; client.buffer[i] != '\0' && client.buffer[i] != '\r' && client.buffer[i] != '\n'; i++)
		pos = i;
	std::string test(":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetHostname() + " " + std::string(client.buffer).erase(pos + 1, -1) + "\r\n");
	std::cout << "send = " << test << std::endl;
	std::cout << "nick = " << client.GetNickname() << std::endl;
	send(client.GetSocket(), test.c_str(), test.length(), 0);
	(void)server;
}