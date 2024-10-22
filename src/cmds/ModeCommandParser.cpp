#include "ModeCommandParser.hpp"
#include <sstream>
#include <vector>

bool ModeCommandParser::parse(const std::string &input, std::vector<std::vector<std::string>>& params)
{
    std::istringstream iss(input);
    std::string token;

    if (!(iss >> token)) {
	return false;
    }
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
    return !params.empty();
}


// std::vector<std::string> parseMode(std::vector<std::string> parsedParams)
// {
//     std::vector<std::string> parsedMode;
//
//     std::string temp;
//     for (std::vector<std::string>::iterator it = parsedParams.begin(); it != parsedParams.end(); ++it)
//     {
// 	if (it == parsedParams.begin())
// 	{
// 	    temp = *it;
// 	}
// 	else if ((*it)[0] == '+' || (*it)[0] == '-')
// 	{
// 	    parsedMode.push_back(temp);
// 	    temp.clear();
// 	    temp = *it;
// 	}
// 	else if ((*it)[0] != '+' && (*it)[0] != '-')
// 	    temp += " " + *it;
// 	if ((it + 1) == parsedParams.end())
// 	    parsedMode.push_back(temp);
//     }
//     std::cout << "MODE PARAM {" << std::endl;
//     for (std::vector<std::string>::iterator it = parsedMode.begin(); it != parsedMode.end(); it++)
//     {
// 	 std::cout << *it << std::endl;
//     }
//     std::cout << "}" << std::endl;
//     return parsedMode;
// }
