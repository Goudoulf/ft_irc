#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"

bool	isConnected(const std::string param, int fd , IRCServer& server)
{
    (void) param;
    Client* client = (server.getClients()->find(fd))->second;
    return (client->GetIsConnected());

}

bool	isAlphaNum(const std::string param, int fd , IRCServer& server)
{
    (void)server;
    for (std::string::const_iterator it2 = param.begin(); it2 != param.end(); it2++)
	if (!isalnum((*it2)))
	{
	    std::string rpl(ERR_BADCHANNELKEY((param)));
	    send(fd, rpl.c_str(), rpl.size(), 0);
	    return false;
	}
    return true;
}
