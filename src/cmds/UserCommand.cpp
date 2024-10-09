#include "UserCommand.hpp"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void UserCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	Client* client = (server.getClients()->find(client_fd))->second;
	log(INFO,"_____user command_____");
	client->SetUsername(params.find("user")->second);
	if (client->GetUsername().size() != 0 && client->GetNickname().size() != 0)
	{
		client->SetPrefix();
		std::string nickname = client->GetNickname();
		std::map<std::string, std::string> param {{"nickname", nickname}};
		reply_server("001", *client, ":Welcome to the local Network " + nickname);
		reply_server("002", *client, ":Your host is " + client->GetHostname() + ", running on NetTwerkers_v0.1");
		reply_server("003", *client, ":This server was created 07/29/2024");
		reply_server("004", *client, ":NetTwerkers_v0.1 - itkol");
	}

}
