#include "NickCommand.hpp"
#include "cmds.h"
#include "debug.h"

void NickCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    log(INFO, "_____nick command_____");
    client->SetNickname(params.find("nick")->second);
}
