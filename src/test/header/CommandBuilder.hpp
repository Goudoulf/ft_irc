#pragma once
#include <string>
#include <utility>
#include "Command.hpp"

class CommandBuilder {

public:

    ~CommandBuilder();
    const std::string getName()const;

    class Builder {

    public: 

        Builder();
        Builder& name(std::string name);
        Builder& param(std::string description, std::string param);
        Builder& command(Command *command);
        
        const CommandBuilder *build() const;

    private:

        std::string _name;
        std::vector<std::pair<std::string, std::string>> _params;
        Command *_command;
    };

protected:

    CommandBuilder(const std::string& name, const std::vector<std::pair<std::string, std::string>>& params, Command *command);

private:

    friend class CommandDirector;
    std::string _name;
    std::vector<std::pair<std::string, std::string>> _params;
    Command *_command;
    void    fill_param(int fd, std::vector<std::string>& param, IRCServer& server)const;

    };
