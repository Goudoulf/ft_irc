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
#include <iostream>

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
    IRCServer server(argv[1], "test");
    server.run();
    return (0);
}
