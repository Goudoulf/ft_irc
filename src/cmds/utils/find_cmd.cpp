#include "cmds.h"
#include <sstream>
#include <string>

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial) {
    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(buffer);
    
    while (std::getline(stream, temp)) {
        if (!temp.empty() && temp.back() == '\r') {
            temp.pop_back();
        }
        lines.push_back(temp);
    }

    if (!buffer.empty() && buffer.back() != '\n') {
        remainingPartial = lines.back();
        lines.pop_back();
    } else {
        remainingPartial.clear();
    }
    return lines;
}
