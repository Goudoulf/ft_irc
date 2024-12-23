#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "Command.hpp"
#include <string>

class ModeCommand : public Command {

public:

    ModeCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
};

#endif
