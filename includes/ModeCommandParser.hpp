#ifndef MODECOMMANDPARSER_HPP
#define MODECOMMANDPARSER_HPP

#include "CommandParser.hpp"

#include <string>
#include <vector>

class ModeCommandParser: public CommandParser {

    public:
	bool parse(const std::string &input, std::vector<std::vector<std::string> >& params);
};

#endif
