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
#include "../../includes/color.h"
#include "../../includes/debug.h"

LogLevel currentLogLevel = DEBUG;

void log(LogLevel level, const std::string message)
{
	if (level >= currentLogLevel)
	{
		switch (level) {
			case DEBUG:
				std::cout << BLUE << "[DEBUG]: " << message << RESET << std::endl;
				break;
			case INFO:
				std::cout << WHITE << "[INFO]: " << message << RESET << std::endl;
				break;
			case CMD:
				std::cout << CYAN << "[CMD]: " << message << RESET << std::endl;
				break;
			case REPLY:
				std::cout << GREEN << "[REPLY]: " << message << RESET << std::endl;
				break;
			case WARN:
				std::cout << YELLOW << "[WARN]: " << message << RESET << std::endl;
				break;
			case ERROR:
				std::cout << RED <<"[ERROR]: " << message << RESET <<std::endl;
				break;
		}
		std::cout << std::flush;
	}
}
