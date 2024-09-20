#include "../../includes/cmds.h"
#include "../../includes/reply.h"
#include "../Channel.hpp"
#include <vector>
#include <string>
#include "../../includes/debug.h"

void	cap(IRCServer &server, int fd, std::vector<std::string>& params)
{
    Client* client = (server.getClients()->find(fd))->second;
    if (params.size() && params[0] == "END")
    {
	log(DEBUG, "Ignoring END");
	return ;
    }
    if (params.size() && params[0] != "LS")
    {
	log(ERROR, "Unknown IRC command: " + params[0]);
	std::map<std::string, std::string> par {{"command", "CAP " + params[0]}};
	sendIRCReply(*client, "421", par);
    }
    if (params.size() > 1 && params[1] != "302")
    {
	log(ERROR, "Unknown IRC command: " + params[1]);
	std::map<std::string, std::string> par {{"command", "CAP LS" + params[1]}};
	sendIRCReply(*client, "421", par);
    }
    reply_server("CAP * LS", *client, ":");
}
