#include "../../includes/cmds.h"

void	client_connect(Client &client)
{
	//std::string temp(client.buffer);
	int sd = client.GetSocket();

	std::cout << "buffer[" << std::endl << client.GetBufferString() << std::endl << "]" << std::endl;
	//client.SetBuffer(client.buffer);
	if (client.GetBufferString().find("USER") != (size_t)-1)
		client.finduser(client.GetBufferString().c_str()); 
	if (client.GetBufferString().find("NICK") != (size_t)-1)
		client.findnick(client.GetBufferString().c_str());
	if (client.GetBufferString().find("USER") != (size_t)-1 && client.GetBufferString().find("USER") != (size_t)-1)
	{
		client.SetPrefix();
		std::string nickname = client.GetNickname();
		std::string rpl(":127.0.0.1 001 " + nickname + " :Welcome to the local Network " + nickname +"\r\n");
		std::cout << "Reply = " << rpl << std::endl;
		send(sd, rpl.c_str(), rpl.length(), 0);
		rpl = ":127.0.0.1 002 " + nickname + " :Your host is " + client.GetHostname() + ", running on NetTwerkers_v0.1\r\n";
		std::cout << "Reply = " << rpl << std::endl;
		send(sd, rpl.c_str(), rpl.length(), 0);
		rpl = ":127.0.0.1 003 " + nickname + " :This server was created 07/29/2024\r\n";
		std::cout << "Reply = " << rpl << std::endl;
		send(sd, rpl.c_str(), rpl.length(), 0);
		rpl = ":127.0.0.1 004 " + nickname + " " + client.GetHostname() + " NetTwerkers_v0.1 - itkol\r\n";
		std::cout << "Reply = " << rpl << std::endl;
		send(sd, rpl.c_str(), rpl.length(), 0);
		client.SetIsConnected(true);
	}
}
