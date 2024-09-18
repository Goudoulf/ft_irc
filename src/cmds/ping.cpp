// TODO: Create ping command

#include "../../includes/cmds.h"
#include "../../includes/debug.h"

void	ping(IRCServer &server, int fd, std::vector<std::string>& params)
{
	(void)params;
    Client* client = (server.getClients()->find(fd))->second;
	log(CMD, client->GetNickname() + ":_____ping_____");
	std::string buf = client->GetBuffer();
	buf = buf.substr(0, buf.find_first_of(" \r\n\0", 5));
	std::cout << buf << "|" << std::endl;
	int sd = client->GetSocket();
	std::string rpl("PONG 127.0.0.1\r\n");
	log(REPLY, "PONG 127.0.0.1 ");
	send(sd, rpl.c_str(), rpl.length(), 0);
}
