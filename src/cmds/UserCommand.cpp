#include "UserCommand.hpp"
#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void UserCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	log(INFO,"_____user command_____");
	IRCServer *server = IRCServer::getInstance();
	client->setUsername(params.find("user")->second.begin()[0]);
	if (client->getUsername().size() != 0 && client->getNickname().size() != 0)
	{
		std::string nickname = client->getNickname();
		client->setPrefix();
		client->replyServer(RPL_WELCOME(nickname));
		client->replyServer(RPL_YOURHOST(nickname));
		client->replyServer(RPL_CREATED(nickname, server->getCreationDate()));
		client->replyServer(RPL_MYINFO(nickname));
		client->setLevel(REGISTERED);
	}
}
