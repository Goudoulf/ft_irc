/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 10:05:26 by lvallini         ###   ########.fr       */
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
#define SHUTDOWN_MSG "shutdown"

IRCServer* IRCServer::_instance = NULL;

void myExit(std::string error, int code)
{
    log(ERROR, error);
    exit(code);
}

IRCServer::~IRCServer(void)
{
    log(INFO, "IRC Server closing");
    delete _director;
    close (_pipefd[0]);
    close (_pipefd[1]);
    delete[] _pipefd;
    removeAllClient();
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
    memset(&_address, 0, sizeof(_address));
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _portString = port;
    _password = password;
    _passwordIsSet = !_password.empty();
    _director = new CommandDirector();
    _pipefd = new int[2];
    setCommandTemplate(_director);
    _addrlen = sizeof(_address);
    _creation_date = getTime();
}

void  IRCServer::initSocket()
{
    int opt = 1;
    pipe(_pipefd);
    if ((_serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        myExit("socket failed", EXIT_FAILURE);
    if (setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        myExit("setsockopt", EXIT_FAILURE);
    setNonBlocking(_serverfd);
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    if (bind(_serverfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        log(ERROR, strerror(errno));
        myExit("bind failed", EXIT_FAILURE);
    }
    if (listen(_serverfd, 10) < 0)
        myExit("listen error", EXIT_FAILURE);
    _epollfd = epoll_create1(0);
    if (_epollfd == -1)
    {
        close(_serverfd);
        myExit("epoll_create1 failed", EXIT_FAILURE);
    }
    _event.events = EPOLLIN;
    _event.data.fd = _serverfd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _serverfd, &_event) == -1)
    {
        close(_serverfd);
        close(_epollfd);
        myExit("epoll_ctl: server_fd failed", EXIT_FAILURE);
    }
    _event.events = EPOLLIN;
    _event.data.fd = _pipefd[0];
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _pipefd[0], &_event) == -1)
        myExit("epoll_ctl failed for pipe", EXIT_FAILURE);
    _clients.insert(std::pair<int, Client*>(_serverfd, NULL));
}

int     IRCServer::run(void)
{
    log(INFO, "IRC Server loop is starting");
    while (!stop)
    {
        int event_count = epoll_wait(_epollfd, _events, MAX_EVENTS, 0);
        if (event_count == -1)
        {
            if (errno == EINTR) 
                continue;
            myExit("epoll_wait failed", EXIT_FAILURE);
        }
        for (int i = 0; i < event_count; i++)
        {
            if (_events[i].data.fd == _serverfd)
                acceptConnection();
            else if (_events[i].data.fd == _pipefd[0])
            {
                char buffer[10];
                read(_pipefd[0], buffer, sizeof(buffer));
                if (strncmp(buffer, SHUTDOWN_MSG, strlen(SHUTDOWN_MSG)) == 0)
                {
                    log (ERROR, "Received shutdown signal");
                    stop = 1;
                    break;
                }
            }
            else
                readData(i);
        }
    }
    return 0;
}

void    IRCServer::readData(int i)
{
    log(INFO, "read data start");
    int clientfd = _events[i].data.fd;
    Client* client = _clients.find(clientfd)->second;
    log(INFO, "Clear buffer");
    bzero(client->getBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((_valread = recv(clientfd, client->getBuffer(), 1024, 0)) <= 0)
    {
		close(clientfd);
        QuitCommand::quitAll(client, "Lost connection");
        log(WARN, "recv: socket closed");
        if (_valread == -1)  
            log(ERROR, "recv: error");
    }
    else
    {
        log(DEBUG, client->getBuffer());
        std::string remainingPartial = client->getPartialBuffer();
        std::string completeBuffer = remainingPartial + client->getBuffer();
        if (completeBuffer.size() > MAX_BUFFER_SIZE)
        {
            log(ERROR, "Buffer overflow from client , disconnecting.");
            QuitCommand::quitAll(client, "Buffer Overflow");
            client->erasePartialBuffer();
            return;
        }
        std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);
        client->setPartialBuffer(remainingPartial);
        log(INFO, "parse command start");
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
            _director->parseCommand(client, *it);
    }
    log(INFO, "epoll_wait start");
}

void    IRCServer::acceptConnection()
{
    int     newSocket;
    log(INFO, "Server accepting new connection");
    newSocket = accept(_serverfd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
    if (newSocket < 0)
        myExit("accept error", EXIT_FAILURE);
    setNonBlocking(newSocket);
    _event.events = EPOLLIN | EPOLLET;
    _event.data.fd = newSocket;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newSocket, &_event) == -1)
    {
        log(ERROR, "epoll_ctl: new socket failed");
        close(newSocket);
    }
    _clients.insert(std::pair<int, Client*>(newSocket, new Client(newSocket, _address)));
}

void    IRCServer::sendReply(int target, std::string message)
{
	log(DEBUG, "REPLY SERVER :" + message);
	send(target, message.c_str(), message.size(), 0);
}

Channel *IRCServer::createChannel(std::string channel, Client *client, std::string key)
{
    _channels.insert(std::pair<std::string, Channel*>(channel, new Channel(channel, client, key)));
    return (_channels.find(channel)->second);
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
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second && it->second->getNickname() == nickname)
            return it->second;
    }
    return (NULL);
}

void	IRCServer::removeAllClient()
{
    std::map<int, Client*>::iterator it; 
    for (it = _clients.begin();it != _clients.end(); ++it)
    {
        if (it->second)
        {
            log(DEBUG, it->second->getNickname() + " is deleted");
            QuitCommand::quitAll2(it->second, "Server Closing");
            if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, it->first, NULL) == -1)
                log(ERROR,"epoll_ctl: EPOLL_CTL_DEL failed");
            close(it->first);
            delete ((it->second->getClient()));
            it->second = NULL;
        }
    }
    _clients.clear();
}

void	IRCServer::removeClient(Client *client)
{
    std::map<int, Client*>::iterator it = _clients.find(client->getSocket());
    if (it != _clients.end() && it->second)
    {
        log(DEBUG, it->second->getNickname() + " is deleted");
        if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, it->first, NULL) == -1)
            log(ERROR,"epoll_ctl: EPOLL_CTL_DEL failed");
        close(it->first);
        delete ((it->second->getClient()));
        _clients.erase(it);
    }
}

void	IRCServer::removeChannel(Channel *channel)
{
    log(DEBUG, channel->getChannelName() + " is trying to delete");
    std::map<std::string, Channel*>::iterator it;
    if ((it = _channels.find(channel->getChannelName())) != _channels.end())
    {
        log(DEBUG, it->second->getChannelName() + " is deleted");
        delete it->second;
        _channels.erase(it);
    }
}

bool	IRCServer::checkNick(const std::string& nick)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
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
    return _portString;
}

bool    IRCServer::getPasswordIsSet()
{
    return _passwordIsSet;
}


int*	IRCServer::getPipeFd()
{
    return _pipefd;
}
