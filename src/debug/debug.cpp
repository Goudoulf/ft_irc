/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:07:57 by cassie            #+#    #+#             */
/*   Updated: 2024/08/26 08:37:11 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <ostream>
#include <string>
#include "color.h"
#include "debug.h"
#define MAX_SIZE 80

LogLevel currentLogLevel = DEBUG;

std::string log_time()
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
				std::cout << "[" << log_time() << "]" << BLUE << "[DEBUG]: " << message << RESET << std::endl;
				break;
			case INFO:
				std::cout << "[" << log_time() << "]" << WHITE << "[INFO]: " << message << RESET << std::endl;
				break;
			case CMD:
				std::cout << "[" << log_time() << "]"<< CYAN << "[CMD]: " << message << RESET << std::endl;
				break;
			case REPLY:
				std::cout << "[" << log_time() << "]"<< GREEN << "[REPLY]: " << message << RESET << std::endl;
				break;
			case WARN:
				std::cout << "[" << log_time() << "]"<< YELLOW << "[WARN]: " << message << RESET << std::endl;
				break;
			case ERROR:
				std::cout << "[" << log_time() << "]"<< RED <<"[ERROR]: " << message << RESET <<std::endl;
				break;
		}
		std::cout << std::flush;
	}
}
