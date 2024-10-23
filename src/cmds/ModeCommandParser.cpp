#include "ModeCommandParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>

bool ModeCommandParser::parse(const std::string &input, std::vector<std::vector<std::string>>& params)
{
    std::istringstream iss(input);
    std::string token, target, tempString;

    if (!(iss >> token))
		return false;
	std::vector<std::string> channel;
	iss >> token;
	channel.push_back(token);
	params.push_back(channel);
    std::vector<std::string>modes;
	iss >> token;
	if (token.at(0) != '+' && token.at(0) != '-')
		return (false);
	tempString += token + " ";
	while (iss >> token)
    {
		if (token.at(0) == '+' || token.at(0) == '-')
		{
			modes.push_back(tempString);
			tempString.erase();
			tempString += token + " ";
		}
		else
			tempString += token + " ";
    }
	modes.push_back(tempString);
	params.push_back(modes);
	// std::cout << "MODE PARAM {" << std::endl;
	// for (std::vector<std::vector<std::string>>::iterator it = params.begin(); it != params.end(); it++)
	// {
	// 	std::cout << "ModeParams: ";
	// 	for (std::vector<std::string>::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
	// 	{
	// 		std::cout << (*it2) << std::endl;
	// 	}
	// }
	// std::cout << "}" << std::endl;
    return !params.empty();
}
