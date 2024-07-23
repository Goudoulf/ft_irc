/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/07/23 12:52:24 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

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
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        my_exit("socket failed", EXIT_FAILURE);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &timeout, sizeof(timeout)))
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
            sd = _it->second->GetSocket();
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }
        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
        if ((activity < 0) && (errno != EINTR))
            my_exit("select error", EXIT_FAILURE);
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
                my_exit("accept error", EXIT_FAILURE);
            // fcntl(new_socket, F_SETFL, fcntl(new_socket, F_GETFL, 0) | O_NONBLOCK);
            _clients.insert(std::pair<std::string, Client*>("temp", new Client(new_socket)));
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &address.sin_addr, ip_str, sizeof(ip_str));
            struct hostent *host = gethostbyname(ip_str);
            _clients["temp"]->SetHostname(ip_str);
            std::cout << _clients["temp"]->GetHostname() << std::endl;
            (void)host;
        }
        for (_it = _clients.begin(); _it != _clients.end(); _it++) {
            sd = _it->second->GetSocket();
            if (FD_ISSET(sd, &readfds)) {
		// int i = 0;
		// int count = 0;
                // do {
                //     valread = recv(sd, _it->second->buffer + i, 1024, 0);
                //     std::cout << "ERRNO=" << std::strerror(errno) << std::endl;
                //     std::cout << "{" << _it->second->buffer << "}" << std::endl;
                //     i += valread;
                //     std::cout << "loope" << valread << std::endl;
                //     count++;
                // }
                // while (i > 2 && (_it->second->buffer[i - 2] != '\r'));
                // while (i > 2 && (_it->second->buffer[i - 1] != '\n' && _it->second->buffer[i - 2] != '\r'));
                    // recv(sd, _it->second->buffer + i, 1024, MSG_DONTWAIT);
                if ((valread = recv(sd, _it->second->buffer, 1024, 0)) == 0) {
                    close(sd);
                    std::cout << "error?" << std::endl;
                    _it->second->SetSocket(0);
                }
                else {
                    std::string temp(_it->second->buffer);
                    std::cout << "buffer[" << std::endl << _it->second->buffer << std::endl << "]" << std::endl;
                    _it->second->SetBuffer(_it->second->buffer);
                    if (temp.find("USER") != (size_t)-1)
                    {
                        _it->second->finduser(temp.c_str());
                    }
                    if (temp.find("NICK") != (size_t)-1)
                    {
                        _it->second->findnick(temp.c_str());
                    }
                    if (_it->first == "temp" && _it->second->GetNickname() != "default" && _it->second->GetUsername() != "default" )
                    {
                        _it->second->SetClient();
                        std::string temp2 = _it->second->GetNickname(); 
                        _clients.insert(std::pair<std::string, Client*>(temp2, _clients.find("temp")->second));
                        _clients.erase("temp");
                        _it = _clients.find(temp2);
                    }
                    if (strncmp(_it->second->buffer, "JOIN", 4) == 0)
                    {
                        int pos;
                        for (int i = 4; _it->second->buffer[i] != '\0' && _it->second->buffer[i] != '\r' && _it->second->buffer[i] != '\n'; i++)
                            pos = i;
                        std::string test(":" + _it->second->GetNickname() + "!" + _it->second->GetUsername() + "@" + _it->second->GetHostname() + " " + std::string(_it->second->buffer).erase(pos + 1, -1) + "\r\n");
						std::cout << "send = " << test << std::endl;
						std::cout << "nick = " << _it->second->GetNickname() << std::endl;
                        send(sd, test.c_str(), test.length(), 0);
                    }
                    //_it->second->buffer[valread] = '\0';
                }
                bzero(_it->second->buffer, 1024);
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
