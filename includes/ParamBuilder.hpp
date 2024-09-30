#pragma once
#include <string>
#include <functional>

class ParamTemplate {
public:

    ParamTemplate& param(const std::string& param);
    ParamTemplate& validate(std::function<bool(const std::string&)> validator);
    const std::string& build();

private:

    std::string _param;
    bool _isValid = true;
};

