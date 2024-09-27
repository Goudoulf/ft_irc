#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> tokenize(const std::string& input, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);
    std::cout << input << std::endl;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
