#include "Command.hpp"
#include <string>
#include <iostream>

class JoinCommand : public Command {

public:

    JoinCommand() {}
    void execute(int client_fd, const std::vector<std::string>& params);
};
