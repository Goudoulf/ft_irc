#ifndef DEFAULTCOMMANDPARSER_HPP
#define DEFAULTCOMMANDPARSER_HPP

#include "CommandParser.hpp"

#include <string>
#include <vector>

class DefaultCommandParser : public CommandParser {

    public:
	bool parse(const std::string &input, std::vector<std::vector<std::string> >& params);
};

#endif
