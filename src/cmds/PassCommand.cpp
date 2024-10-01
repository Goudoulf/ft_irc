#include "cmds.h"
#include "debug.h"
#include "reply.h"
#include "PassCommand.hpp"

void PassCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
	std::cout << "_____pass_____" << std::endl;
	if (params.size() < 1)
	{
		log(ERROR, "Wrong Number of param");
			return ;
	}
	if (params.find("pass")->second == server.getPassword())
	{
		client->SetIsConnected(true);
		return ;
	}
	log(ERROR, "Wrong password");
	std::map<std::string, std::string> par {{"command", "PASS"}};
	return ;
}
