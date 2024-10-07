#pragma once
#include <string>
#include <utility>
#include "Command.hpp"
#include "ParamTemplate.hpp"

class Command;
class ParamTemplate;

class TemplateBuilder {

public:

    ~TemplateBuilder();
    const std::string getName()const;

    class Builder {

    public: 

        Builder();
        Builder& name(std::string name);
        Builder& param(std::string type, const ParamTemplate *checker);
        Builder& trailing(std::string param, const ParamTemplate *checker);
        Builder& command(Command *command);
        
        const TemplateBuilder *build() const;

    private:

        std::string _name;
        std::vector<std::pair<std::string, const ParamTemplate*>> _params;
        Command *_command;
    };

protected:

    TemplateBuilder(const std::string& name, const std::vector<std::pair<std::string, const ParamTemplate*>>& _params, Command *command);

private:

    friend class CommandDirector;
    std::string _name;
    std::vector<std::pair<std::string, const ParamTemplate*>> _params;
    Command *_command;
    void    fill_param(int fd, std::vector<std::string>& param, IRCServer& server)const;

    };
