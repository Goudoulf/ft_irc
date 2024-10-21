#pragma once
#include <string>
#include <vector>

class CommandParser
{

    public:
	virtual ~CommandParser() {}
	virtual bool parse(const std::string &input, std::vector<std::vector<std::string>> & params) = 0;
};
