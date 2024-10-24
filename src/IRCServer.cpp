/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/10/21 15:47:47 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "CmdLevel.h"
#include "ircserv.h"
#include "Client.hpp"
#include "debug.h"
#include "cmds.h"
#include "CommandDirector.hpp"
#include <cmath>
#include <cstring>
#include <sys/select.h>
#include <unistd.h>
#include <utility>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>


extern bool stop;
const size_t MAX_BUFFER_SIZE = 512;

IRCServer* IRCServer::_instance = nullptr;



std::map<int, std::string> clientPartialBuffers;

void my_exit(std::string error, int code)
{
    log(ERROR, error);
    exit(code);
}

IRCServer::~IRCServer(void)
{
}

IRCServer*	IRCServer::getInstance()
{
    if (_instance == nullptr)
        _instance = new IRCServer();
    return _instance;
}

void	IRCServer::initialize(std::string port, std::string password)
{
    char *end;
    log(DEBUG, "IRC Server is setting up socket");
    memset(&address, 0, sizeof(address));
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _port_string = port;
    _password = password;
    _passwordIsSet = !_password.empty();
    _director = new CommandDirector();
    setCommandTemplate(_director);

    addrlen = sizeof(address);
    _creation_date = getTime();
}

void  IRCServer::initSocket()
{
    int test = 1;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        my_exit("socket failed", EXIT_FAILURE);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &test, sizeof(test)))
        my_exit("setsockopt", EXIT_FAILURE);

    setNonBlocking(server_fd);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        log(ERROR, strerror(errno));
        my_exit("bind failed", EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
        my_exit("listen error", EXIT_FAILURE);

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN; // Wait for incoming connections
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    _clients.insert(std::pair<int, Client*>(server_fd, NULL));
}

int     IRCServer::run(void)
{
    log(INFO, "IRC Server loop is starting");
    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // Wait indefinitely for events
        if (event_count == -1)
            my_exit("epoll_wait failed", EXIT_FAILURE);
        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_fd)
                acceptConnection();
            else
                readData(i);
        }
    }
    return 0;
}

void		IRCServer::stopServer()
{
        return;
}

void    IRCServer::readData(int i)
{
    int client_fd = events[i].data.fd;
    Client* client = _clients.find(client_fd)->second;
    log(INFO, "Clear buffer");
    bzero(client->GetBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((valread = recv(client_fd, client->GetBuffer(), 1024, 0)) <= 0) {
        removeClient(client);
        log(WARN, "recv: socket closed");
        if (valread == -1)  
            log(ERROR, "recv: error");
    }
    else
    {
        log(DEBUG, client->GetBuffer());
        std::string& clientPartial = clientPartialBuffers[i];
        std::string completeBuffer = clientPartial + client->GetBuffer();
        if (completeBuffer.size() > MAX_BUFFER_SIZE) {
            log(ERROR, "Buffer overflow from client , disconnecting.");
            removeClient(client);
            clientPartialBuffers.erase(client_fd);
            return;
        }

        std::string remainingPartial;
        std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);
        clientPartialBuffers[client_fd] = remainingPartial;
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
            _director->parseCommand(client, *it);
    }
}

void    IRCServer::acceptConnection()
{
    int     new_socket;
    log(INFO, "Server accepting new connection");
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    std::cout << new_socket << std::endl;
    if (new_socket < 0)
        my_exit("accept error", EXIT_FAILURE);
    setNonBlocking(new_socket);
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = new_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1)
    {
        log(ERROR, "epoll_ctl: new socket failed");
        close(new_socket);
    }
    _clients.insert(std::pair<int, Client*>(new_socket, new Client(new_socket, address)));
}


void    IRCServer::sendReply(int target, std::string message)
{
	log(DEBUG, "REPLY SERVER :" + message);
	send(target, message.c_str(), message.size(), 0);
}

Channel *IRCServer::createChannel(std::string channel, Client *client, std::string key)
{
    _channels.push_back(new Channel(channel, client, key));
    return (_channels.back());
}

Channel	*IRCServer::findChannel(std::string channel)
{
    std::vector<Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++) {
        if ((*it)->getChannelName() == channel)
            return (*it);
    }
    return (NULL);
}

Client	*IRCServer::findClient(std::string nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second && it->second->GetNickname() == nickname)
            return it->second;
    }
    return (NULL);
}

void	IRCServer::removeClient(Client *client)
{
    std::map<int, Client*>::iterator it = _clients.find(client->GetSocket());
    if (it != _clients.end() && it->second && it->second->GetNickname() == client->GetNickname())
    {
        log(DEBUG, it->second->GetNickname() + " is deleted");
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, nullptr) == -1)
            perror("epoll_ctl: EPOLL_CTL_DEL failed");
        close(it->first);
        delete ((it->second->GetClient()));
        it = _clients.erase(it);
    }
}

void	IRCServer::removeChannel(Channel *channel)
{
    std::vector<Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (*it && (*it) == channel)
        {
            delete (*it);
            it = _channels.erase(it);
            return ;
        }
    }
}


bool	IRCServer::checkNick(const std::string& nick)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second && it->second->GetNickname() == nick)
                    return false;
	}
    return true;
}

std::string	IRCServer::getPassword()
{
    return _password;
}

std::map<int, Client*> *IRCServer::getClients()
{
    return &_clients;
}

std::vector<Channel*> *IRCServer::getChannels()
{
    return &_channels;
}

std::string	IRCServer::getCreationDate()
{
    return _creation_date;
}

std::string	IRCServer::getPort()
{
    return _port_string;
}

bool    IRCServer::getPasswordIsSet()
{
    return _passwordIsSet;
}
