/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:23:27 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 13:32:47 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "debug.h"
#include <cctype>
#include <cstring>
#include <signal.h>
#include <iostream>
#include <sstream>


static void	sigquit_handler(int sig)
{
    (void)sig;
    IRCServer *server = IRCServer::getInstance();
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
    log(INFO, "Checking port validity");
    if (port.length() < 1 || port.length() > 5)
    {
        log(ERROR, "Port is not valid");
        return false;
    }
    for (size_t i = 0; port[i] != 0; i++)
    {
        if (!isalnum(port[i]))
        {
            log(ERROR, "Port is not valid");
            return false;
        }
    }
    double port_double;
    std::stringstream ss(port); 
    ss >> port_double;

    if (ss.fail())
    {
        log(ERROR, "Port is not valid");
        return false;
    }
    else if (port_double < 6660 || port_double > 6669)
    {
        log(ERROR, "Error Port must be between 6660-6669");
        return false;
    }
    return true;
}

bool isValidPassword(const std::string& password)
{
    log(INFO, "Checking password validity");
    if (password.length() < 1 || password.length() > 23)
    {
        log(ERROR, "Password is not valid");
        return false;
    }
    for (size_t i = 0; password[i] != 0; i++) {
        unsigned char uc = static_cast<unsigned char>(password[i]);

        if (!((uc >= 0x01 && uc <= 0x05) ||
              (uc >= 0x07 && uc <= 0x08) ||
              (uc == 0x0C) ||
              (uc >= 0x0E && uc <= 0x1F) ||
              (uc >= 0x21 && uc <= 0x7F))) {
            log(ERROR, "Port is not valid");
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv)
{
    if (argc < 3 || argc > 5)
    {
        log(ERROR, "Wrong number of arguments: ./ircserv <port> <password>");
        return (1);
    }
    if (argc == 4 && std::strcmp(argv[3], "--debug") == 0)
        currentLogLevel = DEBUG;
    else if (argc == 4 && std::strcmp(argv[3], "--debug") != 0)
    {
        log(ERROR, "Wrong argument: ./ircserv <port> <password> --debug");
        return (1);
    }
    else
        currentLogLevel = INFO;
    signal_handling();
    // signal(SIGPIPE, SIG_IGN);
    log(INFO, "IRC Server launching");
    if (isValidPassword(argv[2]) && isValidPort(argv[1]))
    {
        IRCServer *server = IRCServer::getInstance();
        server->initialize(argv[1], argv[2]);
        if (server->initSocket())
	        server->run();
        delete server;
        return (0);
    }
    return (1);
}
