#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "Command.hpp"
#include <string>

class QuitCommand : public Command {

public:

    QuitCommand() {}
    void execute(Client *client, const std::map<std::string, std::vector<std::string> >& params);
    static void quitAll(Client *client, std::string message);
    static void quitAll2(Client *client, std::string message);
};

#endif
