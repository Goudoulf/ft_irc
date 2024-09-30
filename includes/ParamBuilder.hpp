#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>

class ParamBuilder {
public:
    ParamBuilder& param(const std::string& param);
    ParamBuilder& validate(std::function<bool(const std::string&)> validator);
    const std::string& build();

private:
    std::string _param;
    bool _isValid = true; // To track if the param is valid
};

// Set the parameter in the ParamBuilder
