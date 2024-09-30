#include "ParamBuilder.hpp"

ParamBuilder& ParamBuilder::param(const std::string& param) {
    this->_param = param;
    return *this;
}

ParamBuilder& ParamBuilder::validate(std::function<bool(const std::string&)> validator) {
    if (!validator(_param)) {
        std::cerr << "Error: Parameter '" << _param << "' is invalid!" << std::endl;
        _isValid = false;
    }
    return *this;
}

const std::string& ParamBuilder::build() {
    if (_isValid) {
        return _param;
    } else {
        throw std::runtime_error("Parameter validation failed!");
    }
}
