#include <string>
#include <map>
#include "JoinCommand.hpp"
#include "NickCommand.hpp"

class CommandBuilder {
    std::string commandType;
    std::map<std::string, std::string> params;

public:
    CommandBuilder& setCommandType(const std::string& type) {
        commandType = type;
        return *this;
    }

    CommandBuilder& addParam(const std::string& key, const std::string& value) {
        params[key] = value;
        return *this;
    }

    Command* build() {
        if (commandType == "NICK")
        {
            return new NickCommand(params["nickname"]);
        }
        else if (commandType == "JOIN")
        {
            return new JoinCommand(params["channel"]);
        }
        
        return 0;  // Unknown command
    }
};
