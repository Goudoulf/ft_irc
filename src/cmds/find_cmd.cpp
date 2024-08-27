#include "../../includes/cmds.h"

void	map_init(std::map<std::string, void (*)(Client&, IRCServer&)> &map_func)
{
	map_func.insert(std::make_pair("JOIN ", &join));
	map_func.insert(std::make_pair("NICK ", &nick));
	map_func.insert(std::make_pair("NOTICES ", &privmsg));
	map_func.insert(std::make_pair("PRIVMSG ", &privmsg));
	map_func.insert(std::make_pair("PASS ", &pass));
	map_func.insert(std::make_pair("QUIT ", &quit));
	// map_func.insert(std::make_pair("KICK", &kick));
	// map_func.insert(std::make_pair("TOPIC", &topic));
	map_func.insert(std::make_pair("PART", &part));
	// map_func.insert(std::make_pair("INVITE", &invite));
	// map_func.insert(std::make_pair("MODE", &mode));
}

void	find_cmd(Client &client, IRCServer &server)
{
	//client.SetBuffer(client.buffer);
	std::string buf = client.GetBufferString();
	std::map<std::string, void (*)(Client&, IRCServer&)> map_func;
	map_init(map_func);
 
	for (std::map<std::string, void (*)(Client&, IRCServer&)>::iterator it = map_func.begin(); it != map_func.end(); it++)
		if (buf.compare(0, it->first.length(), it->first) == 0)
			it->second(client, server);
}
