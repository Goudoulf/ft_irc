#pragma once
#include <string>
#include "JoinCommand.hpp"
#include "NickCommand.hpp"

class CommandBuilder {

public:

    Command* build(const std::string& command) {
        if (command == "NICK")
        {
            return new NickCommand();
        }
        else if (command == "JOIN")
        {
            return new JoinCommand();
        }
        return 0;  // Unknown command
    }
};
