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
#include "QuitCommand.hpp"
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

IRCServer* IRCServer::_instance = NULL;

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
    if (_instance == NULL)
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
    bzero(client->getBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((valread = recv(client_fd, client->getBuffer(), 1024, 0)) <= 0) {
        QuitCommand::quitAll(client, "Lost connection");
        log(WARN, "recv: socket closed");
        if (valread == -1)  
            log(ERROR, "recv: error");
    }
    else
    {
        log(DEBUG, client->getBuffer());
        std::string& clientPartial = clientPartialBuffers[i];
        std::string completeBuffer = clientPartial + client->getBuffer();
        if (completeBuffer.size() > MAX_BUFFER_SIZE) {
            log(ERROR, "Buffer overflow from client , disconnecting.");
            QuitCommand::quitAll(client, "Buffer Overflow");
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
    // _channels.push_back(new Channel(channel, client, key));
    _channels[channel] = new Channel(channel, client, key);
    return (_channels[channel]);
}

Channel	*IRCServer::findChannel(std::string channel)
{
    if (_channels.find(channel) != _channels.end())
        return _channels[channel];
    return (NULL);
}

Client	*IRCServer::findClient(std::string nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second && it->second->getNickname() == nickname)
            return it->second;
    }
    return (NULL);
}

void	IRCServer::removeClient(Client *client)
{
    std::map<int, Client*>::iterator it = _clients.find(client->getSocket());
    if (it != _clients.end() && it->second)
    {
        log(DEBUG, it->second->getNickname() + " is deleted");
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, NULL) == -1)
            log(ERROR,"epoll_ctl: EPOLL_CTL_DEL failed");
        close(it->first);
        delete ((it->second->getClient()));
        _clients.erase(it);
    }
}

void	IRCServer::removeChannel(Channel *channel)
{
    std::map<std::string, Channel*>::iterator it;
    if ((it = _channels.find(channel->getChannelName())) != _channels.end())
    {
        delete _channels[channel->getChannelName()];
        _channels.erase(it);
    }
}

bool	IRCServer::checkNick(const std::string& nick)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second && it->second->getNickname() == nick)
                    return false;
	}
    return true;
}

std::string IRCServer::getPassword()
{
    return _password;
}

std::map<int, Client*> *IRCServer::getClients()
{
    return &_clients;
}

std::map<std::string, Channel*> *IRCServer::getChannels()
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
