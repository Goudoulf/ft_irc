#include "IRCServer.hpp"
#include <cctype>
#include <sys/socket.h>
#include <vector>
#include <string>
#include "reply.h"

bool	isAlphaNum(const std::vector<std::string> param, int fd)
{
    for (std::vector<std::string>::const_iterator it = param.begin(); it != param.end(); it++)
    {
	for (std::string::const_iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
	    if (!isalnum((*it2)))
	    {
		std::string rpl(ERR_BADCHANNELKEY((*it)));
		send(fd, rpl.c_str(), rpl.size(), 0);
		return false;
	    }
    }
    return true;
}
