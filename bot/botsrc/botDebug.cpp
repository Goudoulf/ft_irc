#include <iostream>
#include <ostream>
#include <string>
#include "../color.h"
#include "../debug.h"
#define MAX_SIZE 80

LogLevel currentLogLevel = DEBUG;

std::string logTime()
{
	time_t timestamp = time( NULL );
    struct tm * pTime = localtime( & timestamp );

    char buffer[ MAX_SIZE ];
    strftime( buffer, MAX_SIZE, "%d/%m/%Y %H:%M:%S", pTime );
	return (buffer);
}

void log(LogLevel level, const std::string message)
{
	if (level >= currentLogLevel)
	{
		switch (level) {
			case DEBUG:
				std::cout << "[" << logTime() << "]" << BLUE << "[DEBUG]: " << message << RESET << std::endl;
				break;
			case INFO:
				std::cout << "[" << logTime() << "]" << WHITE << "[INFO]: " << message << RESET << std::endl;
				break;
			case REPLY:
				std::cout << "[" << logTime() << "]"<< GREEN << "[REPLY]: " << message << RESET << std::endl;
				break;
			case WARN:
				std::cout << "[" << logTime() << "]"<< YELLOW << "[WARN]: " << message << RESET << std::endl;
				break;
			case ERROR:
				std::cout << "[" << logTime() << "]"<< RED <<"[ERROR]: " << message << RESET << std::endl;
				break;
		}
		std::cout << std::flush;
	}
}