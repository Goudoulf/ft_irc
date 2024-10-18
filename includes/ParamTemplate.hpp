#pragma once
#include <functional>
#include <string>
#include <vector>
#include "reply.h"
#include "IRCServer.hpp"

class ParamTemplate {

public:

    ~ParamTemplate();
    const std::string getValidity()const;

    class Builder {

    public: 

        Builder();
        Builder& addChecker(bool (*ptr)(const std::string, int fd , IRCServer& server));
        Builder& isOptional();
        const ParamTemplate *build() const;

    private:

        // bool (Builder::*pmf)(const std::vector<std::string&>);
        std::vector<bool (*)(const std::string, int fd , IRCServer& server)> _paramChecker;
        bool _isOptional;
    };

protected:

    ParamTemplate(std::vector<bool (*)(const std::string, int fd , IRCServer& server)> checker, bool optional);

private:

    friend class CommandDirector;
    friend class TemplateBuilder;
    bool    _isValid;
    bool _isOptional;
    std::vector<std::string>    _param;
    std::vector<bool (*)(const std::string, Client *client)> _paramCheckers;
    bool    checkParam(Client* client, const std::string& param)const;
    };
