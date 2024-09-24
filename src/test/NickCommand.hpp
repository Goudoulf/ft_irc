#include "Command.hpp"
#include <string>

class Command;

class NickCommand : public Command {
public:

    NickCommand() {} 
    void execute(int client_fd, const std::vector<std::string>& params);
};
