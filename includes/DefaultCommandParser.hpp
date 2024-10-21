#pragma once

#include "CommandParser.hpp"
#include <string>
#include <vector>

class DefaultCommandParser : public CommandParser {

    public:
	bool parse(const std::string &input, std::vector<std::vector<std::string>>& params);
};
