#include "Command.hpp"
#include <string>
#include <iostream>

class JoinCommand : public Command {
    std::string channel;
public:
    JoinCommand(const std::string& chan) : channel(chan) {}

    void handle(int client_fd) const override {
        std::cout << "Client " << client_fd << " joins channel: " << channel << std::endl;
    }
};
