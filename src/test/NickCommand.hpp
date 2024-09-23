#include "Command.hpp"
#include <string>
#include <iostream>

class NickCommand : public Command {
private:
    std::string nickname;
public:
    NickCommand(const std::string& nick) : nickname(nick) {}

    void handle(int client_fd) const override {
        std::cout << "Client " << client_fd << " changes nickname to: " << nickname << std::endl;
    }
};
