#include "InviteCommand.hpp"

void InviteCommand::execute(Client *client, std::map<std::string, std::vector<std::string>>& params)
{
    std::string target = params.find("nickname")->second;
    std::string channelTarget = params.find("channel")->second;
    Client* clientTarget = server.findClient(target);
    std::string prefixSend = server.getClients()->find(client_fd)->second->GetPrefix();

    rpl_send(client_fd, RPL_INVITING(channelTarget, target));
    rpl_send(clientTarget->GetSocket(), RPL_INVITED(channelTarget, target, prefixSend));
}
