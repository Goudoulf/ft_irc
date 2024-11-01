#include "KickCommandParser.hpp"

#include "cmds.h"

#include <sstream>
#include <vector>

bool KickCommandParser::parse(const std::string &input, std::vector<std::vector<std::string> >& params)
{
    std::istringstream iss(input);
    std::string token;
    std::string command;

    iss >> command;
	//    if (!(iss >> token))
	// return false;
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
	else
	    temp = split(token, ',');
	params.push_back(temp);
    }
    return true;
}
