#include "InviteCommand.hpp"

void InviteCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    (void)server;
    rpl_send(client_fd, RPL_INVITING(params.find("channel")->second, params.find("nickname")->second));
}
