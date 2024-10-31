
#include "PingCommand.hpp"
#include "cmds.h"
#include "debug.h"

void PingCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	(void)params;
	log(CMD, client->getNickname() + ":_____ping_____");
	std::string buf = client->getBuffer();
	buf = buf.substr(0, buf.find_first_of(" \r\n\0", 5));
	log(INFO, buf + "|");
	int sd = client->getSocket();
	std::string rpl("PONG 127.0.0.1\r\n");
	log(REPLY, "PONG 127.0.0.1 ");
	rplSend(sd, rpl);
}
