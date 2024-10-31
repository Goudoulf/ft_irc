#include "DefaultCommandParser.hpp"

#include <sstream>
#include <vector>

bool DefaultCommandParser::parse(const std::string &input, std::vector<std::vector<std::string> >& params)
{
    std::istringstream iss(input);
    std::string token;
    std::string command;

    iss >> command;
    while (iss >> token)
    {
		std::vector<std::string> temp;
		if (token[0] == ':')
		{
			std::string trailing;
			std::getline(iss, trailing);
			temp.push_back(token.substr(1) + trailing);
			params.push_back(temp);
			break;
		} 
		temp.push_back(token);
		params.push_back(temp);
    }
    return true;
}
