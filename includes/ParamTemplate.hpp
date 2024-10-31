#ifndef PARAMTEMPLATE_HPP
#define PARAMTEMPLATE_HPP

#include "Client.hpp"

#include <string>
#include <vector>

class Client;

class ParamTemplate {

public:

    ~ParamTemplate();
    const std::string getValidity()const;

    class Builder {

    public: 

        Builder();
        Builder& addChecker(bool (*ptr)(const std::string, Client *));
        Builder& isOptional();
        const ParamTemplate *build() const;

    private:

        // bool (Builder::*pmf)(const std::vector<std::string&>);
        std::vector<bool (*)(const std::string, Client*)> _paramChecker;
        bool _isOptional;
    };

protected:

    ParamTemplate(std::vector<bool (*)(const std::string, Client*)> checker, bool optional);

private:

    friend class CommandDirector;
    friend class TemplateBuilder;
    bool _isValid;
    bool _isOptional;
    std::vector<std::string>    _param;
    std::vector<bool (*)(const std::string, Client *)> _paramCheckers;
    bool    checkParam(Client* client, const std::vector<std::string> param)const;
    };

#endif
