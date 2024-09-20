#include "../../includes/reply.h"
#include "../../includes/cmds.h"

void	user(IRCServer &server, int fd, std::vector<std::string>& params)
{
	Client* client = (server.getClients()->find(fd))->second;
	std::cout << "_____user command_____" << std::endl;
	client->SetUsername(params[0]);
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
		client->SetIsConnected(true);
	}
}
