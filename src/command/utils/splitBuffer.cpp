#include "cmds.h"
#include <sstream>
#include <string>

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial) {
    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(buffer);  

    while (std::getline(stream, temp))
    {
        if (!temp.empty() && temp[temp.size() - 1] == '\r')
            temp.erase(temp.size() - 1);
        lines.push_back(temp);
    }
    if (!buffer.empty() && buffer[buffer.size() - 1] != '\n')
    {
        if (!lines.empty())
        {
            remainingPartial = lines.back();
            lines.pop_back(); 
        }
        else
            remainingPartial = buffer; 
    }
    else
        remainingPartial.clear(); 
    return lines;
}
