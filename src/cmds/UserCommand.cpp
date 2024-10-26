#include "UserCommand.hpp"
#include "CmdLevel.h"
#include "cmds.h"
#include "debug.h"
#include "reply.h"

void UserCommand::execute(Client *client, const std::map<std::string, std::vector<std::string>>& params)
{
	log(INFO,"_____user command_____"+ params.find("user")->second.begin()[0]);
    IRCServer *server = IRCServer::getInstance();
	client->setUsername(params.find("user")->second.begin()[0]);
	if (client->getUsername().size() != 0 && client->getNickname().size() != 0)
	{
		client->setPrefix();
		std::string nickname = client->getNickname();
		// reply_server("001", *client, ":Welcome to the local Network " + nickname);
		client->replyServer(RPL_WELCOME(nickname));
		client->replyServer(RPL_YOURHOST(nickname));
		client->replyServer(RPL_CREATED(nickname, server->getCreationDate()));
		client->replyServer(RPL_MYINFO(nickname));
		// reply_server("002", *client, ":Your host is " + client->getHostname() + ", running on NetTwerkers_v0.1");
		// reply_server("003", *client, ":This server was created 07/29/2024");
		// reply_server("004", *client, ":NetTwerkers_v0.1 - itkol");
		client->setLevel(REGISTERED);
		log(DEBUG,"_____user command_____ user=" + client->getUsername() + " nick=" + client->getNickname());
	}

}
