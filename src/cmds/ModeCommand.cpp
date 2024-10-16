
#include "ModeCommand.hpp"

// void	mode_invite(bool sign, std::vector<std::string> params, Client& client, IRCServer& server)
// {
//
// 	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
// 	{
// 		Channel *chan = server.find_channel(*it);
// 		std::cout << *it << std::endl;
// 		if (!chan)
// 		{
// 			std::cout << "error chan not found" << std::endl; //ERR_NOSUCHCHANNEL (403)
// 			continue;
// 		}
// 		if (chan->getChannelMode() == noMode)
// 		{
// 			std::cout << "no mode chan" << std::endl;
// 			continue;
// 		}
// 		if (!chan->isOp(client))
// 		{
// 			std::cout << "user not ope" << std::endl; // ERR_CHANOPRIVSNEEDED (482)
// 			continue;
// 		}
// 		chan->setInviteOnly(sign);
// 		std::cout << "invite" << std::endl;
// 	}
// 	// (void)sign;
// 	// (void)params;
// 	// (void)client;
// 	// (void)server;
// }
//
// void	mode_topic(bool sign, std::vector<std::string> params, Client& client, IRCServer& server)
// {
// 	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
// 	{
// 		Channel *chan = server.find_channel(*it);
// 		std::cout << *it << std::endl;
// 		if (!chan)
// 		{
// 			std::cout << "error chan not found" << std::endl; //ERR_NOSUCHCHANNEL (403)
// 			continue;
// 		}
// 		if (chan->getChannelMode() == noMode)
// 		{
// 			std::cout << "no mode chan" << std::endl;
// 			continue;
// 		}
// 		if (!chan->isOp(client))
// 		{
// 			std::cout << "user not ope" << std::endl; // ERR_CHANOPRIVSNEEDED (482)
// 			continue;
// 		}
// 		chan->setTopicRestrictions(sign);
// 		std::cout << "topic" << std::endl;
// 	}
// }
//
// void	mode_key(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
// {
// 	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
// 		std::cout << "key" << std::endl;
// 	(void)plus;
// 	(void)params;
// 	(void)client;
// 	(void)server;
// }
//
// void	mode_ope(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
// {
// 	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
// 	{
// 		Channel *chan = server.find_channel(*it);
// 		std::cout << *it << std::endl;
// 		if (!chan)
// 		{
// 			std::cout << "error chan not found" << std::endl; //ERR_NOSUCHCHANNEL (403)
// 			continue;
// 		}
// 		if (chan->getChannelMode() == noMode)
// 		{
// 			std::cout << "no mode chan" << std::endl;
// 			continue;
// 		}
// 		if (!chan->isOp(client))
// 		{
// 			std::cout << "user not ope" << std::endl; // ERR_CHANOPRIVSNEEDED (482)
// 			continue;
// 		}
// 		std::cout << "ope" << std::endl;
// 	}
// }
//
// void	mode_limit(bool plus, std::vector<std::string> params, Client& client, IRCServer& server)
// {
// 	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
// 		std::cout << "limit" << std::endl;
// 	(void)plus;
// 	(void)params;
// 	(void)client;
// 	(void)server;
// }
//
// void	mapModeInit(std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)> &map_func)
// {
// 	map_func.insert(std::make_pair("i", &mode_invite));
// 	map_func.insert(std::make_pair("t", &mode_topic));
// 	map_func.insert(std::make_pair("k", &mode_key));
// 	map_func.insert(std::make_pair("o", &mode_ope));
// 	map_func.insert(std::make_pair("l", &mode_limit));
// }
//
//
//
// /**	
//  * while +/-
//  * 	-> while "i,t,k,o,l"
//  * 		-> while str -> apply cmd on str
//  */
//
// std::map<std::string, std::string>::iterator nextArgs(std::map<std::string, std::string>::iterator &it, std::map<std::string, std::string> &tokens)
// {
// 	for (std::map<std::string, std::string>::iterator it2 = it; it2 != tokens.end(); it2++)
// 	{
// 		if (it2->second.compare(0, 1, "-") == 0 || it2->second.compare(0, 1, "+") == 0)
// 			return (it2);
// 	}
// 	return (tokens.end());
// }

void ModeCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	(void)server;
	(void)client_fd;
	(void)params;
    Client* client = (server.getClients()->find(client_fd))->second;
	std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)> mapFunc;
	mapModeInit(mapFunc);
	for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); it++)
	{
		if (it->second.compare(0, 1, "+") == 0 || it->second.compare(0, 1, "-") == 0)
		{
			for (unsigned int i = 1; i < it->second.length(); i++)
			{
				for (std::map<std::string, void (*)(bool, std::vector<std::string>, Client&, IRCServer&)>::iterator it2 = mapFunc.begin(); it2 != mapFunc.end(); it2++)
				{
					if (it->second.compare(i, 1, it2->first) == 0)
					{
						std::map<std::string, std::string>::iterator it3 = it++;
						if (it3 != params.end()) //give list of modes if no modes given 
						{
							std::map<std::string, std::string> args(it3, nextArgs(it3, params));
							//it2->second(it->second.compare(0, 1, "+") == 0, args, *client, server);
						}
					}
				}
			}
		}
	}
}
