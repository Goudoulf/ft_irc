#pragma once
#include <string>
#include <vector>

class ParamTemplate {

public:

    ~ParamTemplate();
    const std::string getName()const;

    class Builder {

    public: 

        Builder();
        Builder& param(std::string param, std::vector<std::string> parser(std::string));
        
        const ParamTemplate *build() const;

    private:

        std::vector<std::string> _params;
    };

protected:

    ParamTemplate(const std::vector<std::string>& params);

private:

    friend class CommandDirector;
    std::string _name;
    std::vector<std::string> _params;
    // void    fill_param(int fd, std::vector<std::string>& param, IRCServer& server)const;
    };
