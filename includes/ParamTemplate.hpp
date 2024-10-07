#pragma once
#include <functional>
#include <string>
#include <vector>
#include "IRCServer.hpp"

class ParamTemplate {

public:

    ~ParamTemplate();
    const std::string getValidity()const;

    class Builder {

    public: 

        Builder();
        Builder& addChecker(bool (*ptr)(const std::string, int fd , IRCServer& server));
        const ParamTemplate *build() const;

    private:

        // bool (Builder::*pmf)(const std::vector<std::string&>);
        std::vector<bool (*)(const std::string, int fd , IRCServer& server)> _paramChecker;
    };

protected:

    ParamTemplate(std::vector<bool (*)(const std::string, int fd , IRCServer& server)> checker);

private:

    friend class CommandDirector;
    friend class TemplateBuilder;
    bool    _isValid;
    std::vector<std::string>    _param;
    std::vector<bool (*)(const std::string, int fd , IRCServer& server)> _paramCheckers;
    bool    checkParam(int fd, const std::string& param, IRCServer& server)const;
    };
