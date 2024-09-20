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
#include "../includes/debug.h"
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

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     std::cout << "Wrond number of arguments" << std::endl;
    //     return (-1);
    // }
    if (argc > 2 && std::strcmp(argv[2], "--debug") == 0)
        currentLogLevel = DEBUG;
    else
        currentLogLevel = INFO;
    log(INFO, "IRC Server launching");
    signal_handling();
    IRCServer *server= new IRCServer(argv[1], "test");
    server->run();
    return (0);
}
