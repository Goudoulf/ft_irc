/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:23:27 by cassie            #+#    #+#             */
/*   Updated: 2024/08/26 08:42:21 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "debug.h"
#include <cctype>
#include <cstring>
#include <signal.h>
#include <iostream>

bool    stop = false;

static void	sigquit_handler(int sig)
{
    IRCServer *server = IRCServer::getInstance();
    (void)sig;
    stop = true;
    write(server->getPipeFd()[1], "shutdown", 8);
}

void	signal_handling(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &sigquit_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
}

bool isValidPort(const std::string& port)
{
    if (port.length() < 1 || port.length() > 5)
        return false;
    for (size_t i = 0; port[i] != 0; i++)
    {
         if (!isalnum(port[i]))
            return false;
    }
    return true;
}

bool isValidPassword(const std::string& password) {
    if (password.length() < 1 || password.length() > 23)
        return false;
    for (size_t i = 0; password[i] != 0; i++) {
        unsigned char uc = static_cast<unsigned char>(password[i]);

        if (!((uc >= 0x01 && uc <= 0x05) ||
              (uc >= 0x07 && uc <= 0x08) ||
              (uc == 0x0C) ||
              (uc >= 0x0E && uc <= 0x1F) ||
              (uc >= 0x21 && uc <= 0x7F))) {
            return false;
        }
    }

    return true; // All checks passed
}

int main(int argc, char **argv)
{
    if (argc < 3 || argc > 5)
    {
        std::cout << "Wrond number of arguments" << std::endl;
        return (-1);
    }
    if (argc == 4 && std::strcmp(argv[3], "--debug") == 0)
        currentLogLevel = DEBUG;
    else if (argc == 4 && std::strcmp(argv[3], "--debug") != 0)
        return (-1);
    else
        currentLogLevel = INFO;
    log(INFO, "IRC Server launching");
    signal_handling();
    if (isValidPassword(argv[2]) && isValidPort(argv[1]))
    {
        IRCServer *server = IRCServer::getInstance();
        server->initialize(argv[1], argv[2]);
        server->initSocket();
	server->run();
        delete server;
    }
    return (0);
}
