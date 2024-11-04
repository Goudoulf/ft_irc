/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 13:07:12 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "QuitCommand.hpp"
#include "Client.hpp"
#include "CommandDirector.hpp"

#include "debug.h"
#include "cmds.h"

#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define SHUTDOWN_MSG "shutdown"

const size_t MAX_BUFFER_SIZE = 512;

IRCServer* IRCServer::_instance = NULL;

bool errorExit()
{
    log(ERROR, strerror(errno));
    return (false);
}

IRCServer::~IRCServer(void)
{
    log(INFO, "IRC Server closing");
    delete _director;
    if (_pipefd[0] != -1)
        close (_pipefd[0]);
    if (_pipefd[1] != -1)
        close (_pipefd[1]);
    delete[] _pipefd;
    removeAllClient();
    if (_serverfd != -1)
        close(_serverfd);
    if (_epollfd != -1)
        close(_epollfd);
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
    _pipefd[0] = -1;
    _pipefd[1] = -1;
    _serverfd = -1;
    _epollfd = -1;
}

bool  IRCServer::initSocket()
{
    int opt = 1;
    if (pipe(_pipefd) == -1)
    {
        _pipefd[0] = -1;
        _pipefd[1] = -1;
        return errorExit();
    }
    if ((_serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        return errorExit();
    if (setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        return errorExit();
    if (fcntl(_serverfd, F_SETFL, O_NONBLOCK) == -1)
        return errorExit();
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    if (bind(_serverfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
        return errorExit();
    if (listen(_serverfd, 10) < 0)
        return errorExit();
    _epollfd = epoll_create1(0);
    if (_epollfd == -1)
        return errorExit();
    _event.events = EPOLLIN;
    _event.data.fd = _serverfd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _serverfd, &_event) == -1)
        return errorExit();
    _event.events = EPOLLIN;
    _event.data.fd = _pipefd[0];
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _pipefd[0], &_event) == -1)
        return errorExit();
    _clients.insert(std::pair<int, Client*>(_serverfd, NULL));
    return (true);
}

int     IRCServer::run(void)
{
    bool stop = 0;
    log(INFO, "IRC Server loop is starting");
    while (!stop)
    {
        int event_count = epoll_wait(_epollfd, _events, MAX_EVENTS, 0);
        if (event_count == -1)
        {
            if (errno == EINTR) 
                continue;
            return errorExit();
        }
        for (int i = 0; i < event_count; i++)
        {
            if (_events[i].data.fd == _serverfd)
            {
                if (!acceptConnection())
                    return errorExit();
            }
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
                if (!readData(i))
                    return errorExit();
        }
    }
    return 0;
}

bool    IRCServer::readData(int i)
{
    log(INFO, "read data start");
    int clientfd = _events[i].data.fd;
    Client* client = _clients.find(clientfd)->second;
    log(INFO, "Clear buffer");
    bzero(client->getBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((_valread = recv(clientfd, client->getBuffer(), 1024, 0)) <= 0)
    {
        QuitCommand::quitAll(client, "Lost connection");
		close(clientfd);
        log(WARN, "recv: socket closed");
        if (errno == ENOMEM)
            return (false);
    }
    else
    {
        log(DEBUG, client->getBuffer());
        std::string remainingPartial = client->getPartialBuffer();
        std::string completeBuffer = remainingPartial + client->getBuffer();
        if (completeBuffer.size() > MAX_BUFFER_SIZE)
        {
            log(WARN, "Buffer overflow from client , disconnecting.");
            QuitCommand::quitAll(client, "Buffer Overflow");
            return (true);
        }
        std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);
        client->setPartialBuffer(remainingPartial);
        log(INFO, "parse command start");
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
        {
            if (_clients.find(clientfd) == _clients.end())
                break;
            _director->parseCommand(client, *it);
        }
    }
    log(INFO, "epoll_wait start");
    return (true);
}

bool    IRCServer::acceptConnection()
{
    int     newSocket;
    log(INFO, "Server accepting new connection");
    newSocket = accept(_serverfd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
    if (newSocket < 0)
        return (false);
    if (fcntl(newSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        close (newSocket);
        return (false);
    }
    _event.events = EPOLLIN | EPOLLET;
    _event.data.fd = newSocket;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newSocket, &_event) == -1)
    {
        close(newSocket);
        return (false);
    }
    _clients.insert(std::pair<int, Client*>(newSocket, new Client(newSocket, _address)));
    return (true);
}

void    IRCServer::sendReply(int target, std::string message)
{
	log(DEBUG, "REPLY SERVER :" + message);
	send(target, message.c_str(), message.length(), MSG_NOSIGNAL);
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
        delete (client);
        client = NULL;
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
