//TODO: Create mode command
#include "../../includes/cmds.h"
#include "../../includes/debug.h"


void	mode_invite(bool sign, std::vector<std::string> params, Client& client, IRCServer& server)
{
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
	{
		Channel *chan = server.find_channel(*it);
		std::cout << *it << std::endl;
		if (!chan)
		{
			std::cout << "error chan not found" << std::endl; //ERR_NOSUCHCHANNEL (403)
			continue;
		}
		if (chan->getChannelMode() == noMode)
		{
			std::cout << "no mode chan" << std::endl;
			continue;
		}
		if (chan->isOp(client.GetUsername()))
		{
			std::cout << "user not ope" << std::endl; // ERR_CHANOPRIVSNEEDED (482)
			continue;
		}
		chan->setInviteOnly(sign);
		std::cout << "invite" << std::endl;
	}
	(void)sign;
	(void)params;
	(void)client;
	(void)server;
}

void	mode_topic(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
{
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		std::cout << "topic" << std::endl;
	(void)plus;
	(void)params;
	(void)client;
	(void)server;
}

void	mode_key(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
{
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		std::cout << "key" << std::endl;
	(void)plus;
	(void)params;
	(void)client;
	(void)server;
}

void	mode_ope(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
{
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		std::cout << "ope" << std::endl;
	(void)plus;
	(void)params;
	(void)client;
	(void)server;
}

void	mode_limit(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
{
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		std::cout << "limit" << std::endl;
	(void)plus;
	(void)params;
	(void)client;
	(void)server;
}

void	mapModeInit(std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)> &map_func)
{
	map_func.insert(std::make_pair("i", &mode_invite));
	map_func.insert(std::make_pair("t", &mode_topic));
	map_func.insert(std::make_pair("k", &mode_key));
	map_func.insert(std::make_pair("o", &mode_ope));
	map_func.insert(std::make_pair("l", &mode_limit));
}



/**	
 * while +/-
 * 	-> while "i,t,k,o,l"
 * 		-> while str -> apply cmd on str
 */

std::vector<std::string>::iterator nextArgs(const std::vector<std::string>::iterator &it, const std::vector<std::string> &tokens)
{
	std::vector<std::string>::iterator result = it + 1;
	std::cout << "start:" << *result << std::endl;
	for (std::vector<std::string>::iterator it2 = it + 1; it2 != tokens.end() && !(it2->compare(0, 1, "-") == 0 || it2->compare(0, 1, "+") == 0); it2++)
	{
		result = it2;
	}
	std::cout << "end:" << *result << std::endl;
	return (result);
}

void	mode(IRCServer &server, int fd, std::vector<std::string>& params)
{
    Client* client = (server.getClients()->find(fd))->second;
	std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)> mapFunc;
	mapModeInit(mapFunc);
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); it++)
	{
		if (it->compare(0, 1, "+") == 0 || it->compare(0, 1, "-") == 0)
		{
			for (unsigned int i = 1; i < it->length(); i++)
			{
				for (std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)>::iterator it2 = mapFunc.begin(); it2 != mapFunc.end(); it2++)
				{
					if (it->compare(i, 1, it2->first) == 0)
					{
						if (it + 1 != params.end()) //give list of modes if no modes given 
							it++;
						std::vector<std::string> args(it, nextArgs(it, params));
						it2->second(it->compare(0, 1, "+") == 0, args, *client, server);
					}
				}
			}
		}
	}
}
