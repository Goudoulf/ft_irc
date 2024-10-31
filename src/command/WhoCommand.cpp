#include "WhoCommand.hpp"
#include "debug.h"

void WhoCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
    log(CMD, client->getNickname() + ":_____who_____");
    (void)params;
}
