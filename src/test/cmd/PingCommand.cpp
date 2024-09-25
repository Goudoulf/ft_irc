
#include "../header/PingCommand.hpp"
#include "../../../includes/cmds.h"
#include "../../../includes/debug.h"

void PingCommand::execute(int client_fd, std::vector<std::pair<std::string, std::string>> params, IRCServer& server)
{
	(void)params;
    Client* client = (server.getClients()->find(client_fd))->second;
	log(CMD, client->GetNickname() + ":_____ping_____");
	std::string buf = client->GetBuffer();
	buf = buf.substr(0, buf.find_first_of(" \r\n\0", 5));
	std::cout << buf << "|" << std::endl;
	int sd = client->GetSocket();
	std::string rpl("PONG 127.0.0.1\r\n");
	log(REPLY, "PONG 127.0.0.1 ");
	send(sd, rpl.c_str(), rpl.length(), 0);
}
