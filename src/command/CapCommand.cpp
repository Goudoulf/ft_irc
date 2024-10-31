#include "CapCommand.hpp"
#include "debug.h"

void CapCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    log(CMD, client->getNickname() + ":_____cap_____");
    (void)client;
    (void)params;
}
