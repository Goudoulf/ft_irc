
#include "../../../includes/cmds.h"
#include "../../../includes/debug.h"
#include "../../../includes/reply.h"
#include "../header/PassCommand.hpp"

void PassCommand::execute(int client_fd, std::map<std::string, std::string>std::map<std::string, std::string> params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
	std::cout << "_____pass_____" << std::endl;
	if (params.size() < 1)
	{
		log(ERROR, "Wrong Number of param");
		std::map<std::string, std::string>std::map<std::string, std::string> par {{"command", "PASS"}};
		sendIRCReply(*client, "461", par);
		return ;
	}
	if (params[0].second == server.getPassword())
	{
		client->SetIsConnected(true);
		return ;
	}
	log(ERROR, "Wrong password");
	std::map<std::string, std::string>std::map<std::string, std::string> par {{"command", "PASS"}};
	sendIRCReply(*client, "464", par);
	return ;
}
