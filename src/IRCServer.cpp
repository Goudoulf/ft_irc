/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/07/30 03:08:55 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "../includes/ircserv.h"

void my_exit(std::string error, int code)
{
    std::cerr << error << std::endl;
    exit(code);
}

IRCServer::IRCServer(std::string port, std::string password)
{
    char *end;
    memset(&address, 0, sizeof(address));
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _password = password;
   
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        my_exit("socket failed", EXIT_FAILURE);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &timeout, sizeof(timeout)))
        my_exit("setsockopt", EXIT_FAILURE);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        my_exit("bind failed", EXIT_FAILURE);
    if (listen(server_fd, 10) < 0)
        my_exit("listen error", EXIT_FAILURE);
    addrlen = sizeof(address);
}

IRCServer::~IRCServer(void)
{
}

void    IRCServer::read_data(fd_set *all_sockets, int i)
{
    (void)all_sockets;
    for (_it = _clients.begin(); _it != _clients.end(); _it++) {
        if ((sd = (*_it)->GetSocket()) != i)
            continue ;
        //int set;
        bzero((*_it)->buffer, 1024);
        if ((valread = recv(sd, (*_it)->buffer, 1024, 0)) == 0) {      
            close(sd);
            std::cout << "recv: socket closed" << std::endl;
            (*_it)->SetSocket(0);
            if (valread == -1)  
                std::cout << "recv: error" << std::endl;
        }
        else {
            // check command et parsing buffer a refaire proprement
            std::string temp((*_it)->buffer);
            std::cout << "buffer[" << std::endl << (*_it)->buffer << std::endl << "]" << std::endl;
            (*_it)->SetBuffer((*_it)->buffer);
            if (temp.find("USER") != (size_t)-1)
                (*_it)->finduser(temp.c_str());
            if (temp.find("NICK") != (size_t)-1)
                (*_it)->findnick(temp.c_str());
            if (temp.find("JOIN") != (size_t)-1)
            {
                int pos;
                for (int i = 4; (*_it)->buffer[i] != '\0' && (*_it)->buffer[i] != '\r' && (*_it)->buffer[i] != '\n'; i++)
                    pos = i;
                std::cout << "GET:" << std::string((*_it)->buffer) << "." << std::endl; 
                std::string test(":" + (*_it)->GetNickname() + "!" + (*_it)->GetUsername() + "@" + (*_it)->GetHostname() + " " + std::string((*_it)->buffer).erase(pos + 1, -1) + "\r\n");
                std::cout << "send = " << test << std::endl;
                std::cout << "nick = " << (*_it)->GetNickname() << std::endl;
                send(sd, test.c_str(), test.length(), 0);
            }
            if (temp.find("PRIVMSG") != (size_t)-1)
            {
                std::string test2(":" + (*_it)->GetNickname() + "!" + (*_it)->GetUsername() + "@" 
                                  + (*_it)->GetHostname() + " " + temp + "\r\n");
                for (_it2 = _clients.begin(); _it2 != _clients.end(); _it2++) {
                    if (_it2 != _it)
                    {
                        std::cout << test2 << std::endl;
                        send((*_it2)->GetSocket(), test2.c_str(), test2.length(), 0);
                    }
                }
            }
        }
    }
}

void    IRCServer::accept_connection(fd_set *all_sockets)
{
    int     new_socket;
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0)
            my_exit("accept error", EXIT_FAILURE);
        FD_SET(new_socket, all_sockets);
        if (new_socket > max_sd)
            max_sd = new_socket;
        _clients.push_back(new Client(new_socket, inet_ntoa(address.sin_addr)));
}

int	IRCServer::run(void)
{
    fd_set readfds;
    fd_set all_sockets;
    FD_ZERO(&readfds);
    FD_ZERO(&all_sockets);
    FD_SET(server_fd, &all_sockets);
    max_sd = server_fd;
    while (true) {
        readfds = all_sockets;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
        if ((activity < 0) && (errno != EINTR))
            my_exit("select error", EXIT_FAILURE);
        if (activity == 0)
        {
            //std::cout << "Wait.."<< std::endl; // Wait until a socket update
            continue;
        }
        int i;
        for (i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &readfds) == 1) // check if any fd i have data waiting 
                break;
        }
        std::cout << "Ready" << std::endl;
        if (i == server_fd)
            accept_connection(&all_sockets);
        else
            read_data(&all_sockets, i);
    }
    return 0;
}
