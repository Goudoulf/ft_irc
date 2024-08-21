#include "../../includes/cmds.h"
#include <algorithm>

void	quit(Client &client, IRCServer &server)
{
	std::cout << "_____quit_____" << std::endl;
	std::string buf = client.GetBufferString();
	std::string param (buf.substr(buf.find(":"), buf.find_first_of(" \r\n\0", 5) - 5));
	std::cout << "PARM =" << param << std::endl;
	for (std::vector<Client*>::iterator _it = server.getClients()->begin(); _it != server.getClients()->end(); _it++) {
			message_server((*_it)->GetNickname(), "QUIT", client, param);
	}
	(void)server;
	(void)client;
}
