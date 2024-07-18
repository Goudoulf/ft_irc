/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/07/17 14:06:05 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/types.h>

IRCServer::IRCServer(std::string port, std::string password)
{
    char *end;
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _password = password;
}

IRCServer::~IRCServer(void)
{

}

int	IRCServer::run(void)
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS] = {0}, max_sd, sd, activity, valread;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[1024];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(address);
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
        // for (int i = 0; i < MAX_CLIENTS; i++) {
        //     sd = client_socket[i];
        //     if (sd > 0) FD_SET(sd, &readfds);
        //     if (sd > max_sd) max_sd = sd;
        // }
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            _clients.insert(_it , std::pair{"goudoulf", Client("goudoulf","cassie","cassie","localhost", "ok", new_socket)});

            // for (int i = 0; i < MAX_CLIENTS; i++) {
            //     if (client_socket[i] == 0) {
            //         client_socket[i] = new_socket;
            //         break;
            //     }
            // }
        }
        for (_it = _clients.begin(); _it != _clients.end(); _it++) {
            sd = _it->second.GetSocket();
            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1024)) == 0) {
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
                        //send(sd, ":ray!roro@localhost JOIN #test\r\n", 33, 0);
                        //send(sd, ":ray!roro@localhost PRIVMSG #test :COUCOU\r\n", 44, 0);
                        //std::cout << "join ok\n";
                    }
                    buffer[valread] = '\0';
                    // std::string test = ":cassie!c@localhost PRIVMSG #test :";
                    // send(sd, ":cassie!~c@localhost JOIN #test\n", 30, 0);
                    //send(sd, buffer, strlen(buffer), 0);
                    std::cout << buffer;
                    // bzero(buffer, 1024);
                }
            }
        }
    }
    return 0;
}
