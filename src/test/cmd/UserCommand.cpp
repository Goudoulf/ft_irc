#include "../header/UserCommand.hpp"
#include "../../../includes/cmds.h"
#include "../../../includes/debug.h"
#include "../../../includes/reply.h"

void UserCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
	params.begin();
	Client* client = (server.getClients()->find(client_fd))->second;
		std::cout << "USER PARAM=" << params.find("user")->second << std::endl;
		
	std::cout << "_____user command_____" << std::endl;
	client->SetUsername(params.find("user")->second);
	//set prefix
	std::cout << "New user =" << client->GetUsername() << "|" << std::endl;
	if (client->GetUsername().size() != 0 && client->GetNickname().size() != 0)
	{
		client->SetPrefix();
		std::string nickname = client->GetNickname();
		std::map<std::string, std::string> param {{"nickname", nickname}};
		sendIRCReply(*client, "001", param );
		sendIRCReply(*client, "002", param );
		sendIRCReply(*client, "003", param );
		sendIRCReply(*client, "004", param );
		// reply_server("001", *client, ":Welcome to the local Network " + nickname);
		// reply_server("002", *client, ":Your host is " + client->GetHostname() + ", running on NetTwerkers_v0.1");
		// reply_server("003", *client, ":This server was created 07/29/2024");
		// reply_server("004", *client, ":NetTwerkers_v0.1 - itkol");
	}

}
