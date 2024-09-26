
#include "../header/NickCommand.hpp"

void NickCommand::execute(int client_fd, std::map<std::string, std::string>& params, IRCServer& server)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    std::cout << "_____nick command_____" << std::endl;
    client->SetNickname(params.find("nick")->second);
    //set prefix
    std::cout << "New nick = " << client->GetNickname() << std::endl;
}
