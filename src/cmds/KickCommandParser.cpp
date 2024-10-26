#include "KickCommandParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>

bool KickCommandParser::parse(const std::string &input, std::vector<std::vector<std::string>>& params)
{
    std::istringstream iss(input);
    std::string token;

    if (!(iss >> token))
		return false;
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
	// for (std::vector<std::vector<std::string>>::iterator it = params.begin(); it != params.end(); it++)
	// {
	// 	std::cout << "DIFF" << std::endl;
	// 	for (std::vector<std::string>::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
	// 	{
	// 		std::cout << "AAA"<< (*it2) << std::endl;
	// 	}
	// }
    return !params.empty();
}
