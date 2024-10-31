#ifndef KICKCOMMANDPARSER_HPP
#define KICKCOMMANDPARSER_HPP 

#include "CommandParser.hpp"
#include <string>
#include <vector>

class KickCommandParser: public CommandParser {

    public:
	bool parse(const std::string &input, std::vector<std::vector<std::string> >& params);
};

#endif
