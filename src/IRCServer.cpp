/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/07/22 14:07:56 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include "client_checker.h"

void my_exit(std::string error, int code)
{
    std::cerr << error;
    exit(code);
}

IRCServer::IRCServer(std::string port, std::string password)
{
    char *end;
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _password = password;
     int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        my_exit("socket failed", EXIT_FAILURE);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        my_exit("setsockopt", EXIT_FAILURE);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        my_exit("bind failed", EXIT_FAILURE);
    if (listen(server_fd, 3) < 0)
        my_exit("listen error", EXIT_FAILURE);
    addrlen = sizeof(address);
}

IRCServer::~IRCServer(void)
{
}

int	IRCServer::run(void)
{
    fd_set readfds;
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;
        

        if (_clients.size())
        {
            _it = _clients.end();
            _it--;
            sd = _it->second.GetSocket();
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
            my_exit("select error", EXIT_FAILURE);
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
                my_exit("accept error", EXIT_FAILURE);
			if (check_char("rayqua0123456789"))
				std::cout << "error" <<std::endl;
			std::string nick = "rayqua0123456789";
			if (nick.length() >= 9)
				nick.erase(9, -1);
			std::cout << nick << std::endl;
            _clients.insert(std::pair<std::string, Client>(nick, Client( new_socket)));
			_clients.rbegin()->second.SetClient(nick,"roro","Romain Jacq","localhost", "ok");

        }
        for (_it = _clients.begin(); _it != _clients.end(); _it++) {
            sd = _it->second.GetSocket();
            if (FD_ISSET(sd, &readfds)) {
                if ((valread = recv(sd, buffer, 1024, 0)) == 0) {
                    close(sd);
                    _it->second.SetSocket(0);
                }
                else {
                    if (strncmp(buffer, "JOIN", 4) == 0)
                    {
                        int pos;
                        for (int i = 0; buffer[i] != '\0' && buffer[i] != '\r' && buffer[i] != '\n'; i++)
                            pos = i;
                        std::string test(":" + _it->second.GetNickname() + "!" + _it->second.GetUsername() + "@localhost " + std::string(buffer).erase(pos + 1, -1) + "\r\n");
                        send(sd, test.c_str(), test.length(), 0);
                    }
                    buffer[valread] = '\0';
                    std::cout << buffer;
                }
            }
        }
    }
    return 0;
}
//send(sd, ":ray!roro@localhost JOIN #test\r\n", 33, 0);
//send(sd, ":ray!roro@localhost PRIVMSG #test :COUCOU\r\n", 44, 0);
//std::cout << "join ok\n";
// std::string test = ":cassie!c@localhost PRIVMSG #test :";
// send(sd, ":cassie!~c@localhost JOIN #test\n", 30, 0);
//send(sd, buffer, strlen(buffer), 0);
// bzero(buffer, 1024);
