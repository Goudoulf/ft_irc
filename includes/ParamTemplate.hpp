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
        Builder& addChecker(bool (*ptr)(const std::vector<std::string>));
        const ParamTemplate *build() const;

    private:

        // bool (Builder::*pmf)(const std::vector<std::string&>);
        std::vector<bool (*)(const std::vector<std::string>)> _paramChecker;
    };

protected:

    ParamTemplate(std::vector<bool (*)(const std::vector<std::string>)> checker);

private:

    friend class CommandDirector;
    bool    _isValid;
    std::vector<std::string>    _param;
    std::vector<bool (*)(const std::vector<std::string>)> _paramCheckers;
    void    checkParam(int fd, std::vector<std::string>& param, IRCServer& server);
    };
