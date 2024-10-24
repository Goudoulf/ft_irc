#include "Client.hpp"

void print_client_list(std::map<int, Client*> client)
{
    for (std::map<int, Client*>::iterator it = client.begin(); it != client.end(); it++) {
        if (it->second)
            std::cout << "USER=" << it->second->GetNickname() << std::endl;
    }
}
