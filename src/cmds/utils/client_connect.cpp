#include "cmds.h"
#include "debug.h"

void	client_connect(Client &client)
{
	//std::string temp(client.buffer);
	// int sd = client.GetSocket();

	log(INFO, "client connect"); 
	std::string buf = "buffer[\n" + client.GetBufferString() + "]";
	log(DEBUG,buf); 
	//client.SetBuffer(client.buffer);
	if (client.GetBufferString().find("USER") != (size_t)-1)
		client.finduser(client.GetBufferString().c_str()); 
	if (client.GetBufferString().find("NICK") != (size_t)-1)
		client.findnick(client.GetBufferString().c_str());
	if (client.GetBufferString().find("USER") != (size_t)-1 && client.GetBufferString().find("NICK") != (size_t)-1)
	{
		client.SetPrefix();
		std::string nickname = client.GetNickname();
		std::string rpl(":127.0.0.1 001 " + nickname + ":Welcome to the local Network " + nickname +"\r\n");
		reply_server("001", client, ":Welcome to the local Network " + nickname);
		reply_server("002", client, ":Your host is " + client.GetHostname() + ", running on NetTwerkers_v0.1");
		reply_server("003", client, ":This server was created 07/29/2024");
		reply_server("004", client, ":NetTwerkers_v0.1 - itkol");
		client.SetIsConnected(true);
	}
}
