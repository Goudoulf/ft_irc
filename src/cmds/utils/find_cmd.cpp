#include "cmds.h"
#include <sstream>
#include <string>

// std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial) {
//     std::vector<std::string> lines;
//     std::string temp;
//     std::istringstream stream(buffer);
//
//     while (std::getline(stream, temp)) {
//         if (!temp.empty() && temp.back() == '\r') {
//             temp.pop_back();
//         }
//         lines.push_back(temp);
//     }
//     if (!buffer.empty() && buffer.back() != '\n') {
//         if (!lines.empty()) {
//             remainingPartial = lines.back();
//             lines.pop_back();
//         } else {
//             remainingPartial = buffer;
//         }
//     } else {
//         remainingPartial.clear();
//     }
//
//     return lines;
// }
std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial) {
    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(buffer);  // To split the buffer line by line

    // Process each line in the buffer
    while (std::getline(stream, temp)) {
        // Check if the line ends with '\r' and remove it
        if (!temp.empty() && temp[temp.size() - 1] == '\r') {
            temp.erase(temp.size() - 1);
        }
        lines.push_back(temp);
    }

    // Handle case where the buffer does not end with a complete line (i.e., no final '\n')
    if (!buffer.empty() && buffer[buffer.size() - 1] != '\n') {
        if (!lines.empty()) {
            // Store the last partial line in `remainingPartial`
            remainingPartial = lines.back();
            lines.pop_back();  // Remove it from the `lines` vector
        } else {
            remainingPartial = buffer;  // If there are no full lines, store the buffer itself
        }
    } else {
        remainingPartial.clear();  // Clear `remainingPartial` if buffer ended with '\n'
    }

    return lines;
}
