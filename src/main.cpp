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
#include <cstring>
#include <signal.h>
#include <iostream>

bool    stop = false;

static void	sigquit_handler(int sig)
{
    (void)sig;
    stop = true;
}

void	signal_handling(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &sigquit_handler;
	sigaction(SIGQUIT, &act, NULL);
}

bool isValidPassword(const std::string& password) {
    if (password.length() < 1 || password.length() > 23) {
        return false;
    }

    for (char c : password) {
        unsigned char uc = static_cast<unsigned char>(c);

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
    if (argc < 2 || argc > 4)
    {
        std::cout << "Wrond number of arguments" << std::endl;
        return (-1);
    }
    if (argc > 2 && std::strcmp(argv[2], "--debug") == 0)
        currentLogLevel = DEBUG;
    else
        currentLogLevel = INFO;
    log(INFO, "IRC Server launching");
    signal_handling();
    if (isValidPassword(argv[2]))
    {
	IRCServer *server= new IRCServer(argv[1], "");
        server->setCommandTemplate();
	server->run();
    }
    return (0);
}
