#ifndef DEBUG_H
#define DEBUG_H
#include <string>

enum LogLevel { DEBUG, INFO, REPLY, WARN , ERROR};
extern LogLevel currentLogLevel;

void log(LogLevel level, const std::string message);

#endif
