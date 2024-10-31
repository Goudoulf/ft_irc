#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "Command.hpp"
#include "Channel.hpp"
#include <string>

class PartCommand : public Command {

public:

    PartCommand() {}
    static void partChannel(Channel *channel, std::string message, Client *client);
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
