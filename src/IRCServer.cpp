/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/08/26 14:46:02 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "../includes/ircserv.h"
#include "Client.hpp"
#include "../includes/debug.h"
#include <cmath>
#include <sys/select.h>
#include <unistd.h>

void my_exit(std::string error, int code)
{
    // std::cerr << error << std::endl;
    log(ERROR, error);
    exit(code);
}

IRCServer::IRCServer(std::string port, std::string password)
{
    char *end;
    log(DEBUG, "IRC Server is setting up socket");
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
    _fds.push_back(server_fd);
}

IRCServer::~IRCServer(void)
{
}

int	IRCServer::run(void)
{
    FD_ZERO(&readfds);
    FD_ZERO(&all_sockets);
    FD_SET(server_fd, &all_sockets);
    max_sd = server_fd;
    log(INFO, "IRC Server loop is starting");
    while (true) {
        readfds = all_sockets;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
        if ((activity < 0) && (errno != EINTR))
            my_exit("select error", EXIT_FAILURE);
        if (activity == 0)
        {
            //log(INFO, "Server waiting for socket update..");
            continue;
        }
        log(INFO, "Server new socket activity");
        int i = 0;
	for (std::vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++) {
            if (FD_ISSET(*it, &readfds) == 1) // check if any fd i have data waiting 
            {
                i = (*it);
                break;
            }    
        }
        log(DEBUG, "Server Ready");
        if (i == server_fd)
            accept_connection(&all_sockets);
        else
            read_data(&all_sockets, i);
    }
    return 0;
}

void    IRCServer::read_data(fd_set *all_sockets, int i)
{
	(void)all_sockets;
	(void)i;
        log(INFO, "Server reading data");
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); _it++) {
            if ((sd = (*it)->GetSocket()) != i)
                continue ;
            else
        {
                _it = it;
                break;
        }
        //int set;
        bzero((*_it)->GetBuffer(), 1024);
        // FIX: Undefined disconnect
        if ((valread = recv(sd, (*_it)->GetBuffer(), 1024, 0)) == 0) {
            close(sd);
            log(WARN, "recv: socket closed");
            (*_it)->SetSocket(0);
            if (valread == -1)  
                log(ERROR, "recv: error");
        }
        else {
            // check command et parsing buffer a refaire proprement
            if ((*_it)->GetIsConnected() == false)
                client_connect(**_it);
            else
                find_cmd(**_it, *this);
        }
    }
}

void    IRCServer::accept_connection(fd_set *all_sockets)
{
    int     new_socket;
    log(INFO, "Server accepting new connection");
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0)
        my_exit("accept error", EXIT_FAILURE);
    FD_SET(new_socket, all_sockets);
    if (new_socket > max_sd)
        max_sd = new_socket;
    _clients.push_back(new Client(new_socket, inet_ntoa(address.sin_addr)));
    _fds.push_back(new_socket);
}

std::vector<Client*> *IRCServer::getClients()
{
    return &_clients;
}

std::vector<Channel*> *IRCServer::getChannels()
{
    return &_channels;
}

Channel *IRCServer::create_channel(std::string channel, Client &client, std::string key)
{
    _channels.push_back(new Channel(channel, client, key));
    return (_channels.back());
}

Channel	*IRCServer::find_channel(std::string channel)
{
    std::vector<Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++) {
        if ((*it)->getChannelName() == channel)
            return (*it);
    }
    return (NULL);
}

void	IRCServer::remove_client(Client &client)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end();) {
		if ((*it)->GetUsername() == client.GetUsername())
                {
                        FD_CLR((*it)->GetSocket(), &all_sockets);
                        close((*it)->GetSocket());
                        delete (((*it)->GetClient()));
			_it = _clients.erase(it);
                        return;
                }
		else
			++it;
	}
}
