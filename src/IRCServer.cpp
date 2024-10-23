/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/10/21 14:30:03 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "CmdLevel.h"
#include "ircserv.h"
#include "Client.hpp"
#include "debug.h"
#include "cmds.h"
#include "CommandDirector.hpp"
#include "TemplateBuilder.hpp"
#include "JoinCommand.hpp"
#include "PartCommand.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"
#include "NickCommand.hpp"
#include "QuitCommand.hpp"
#include "ModeCommand.hpp"
#include "ModeCommandParser.hpp"
#include "TopicCommand.hpp"
#include "InviteCommand.hpp"
#include "KickCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "NamesCommand.hpp"
#include "WhoCommand.hpp"
#include "WhoisCommand.hpp"
#include "PingCommand.hpp"
#include "PongCommand.hpp"
#include "CapCommand.hpp"
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

IRCServer*	IRCServer::getInstance()
{
    if (_instance == nullptr)
        _instance = new IRCServer();
    return _instance;
}
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void	IRCServer::initialize(std::string port, std::string password)
{
    char *end;
    log(DEBUG, "IRC Server is setting up socket");
    memset(&address, 0, sizeof(address));
    int test = 1;
    _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
    _port_string = port;
    _password = password;
    _passwordIsSet = !_password.empty();

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

    addrlen = sizeof(address);
    _clients.insert(std::pair<int, Client*>(server_fd, NULL));
    _creation_date = set_time();
}

// void	IRCServer::initialize(std::string port, std::string password)
// {
//     char *end;
//     log(DEBUG, "IRC Server is setting up socket");
//     memset(&address, 0, sizeof(address));
//     int test = 1;
//     _port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
//     _port_string = port;
//     _password = password;
//     _passwordIsSet = !_password.empty();
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//         my_exit("socket failed", EXIT_FAILURE);
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &test, sizeof(test)))
//         my_exit("setsockopt", EXIT_FAILURE);
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(_port);
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
//     {
//         log(ERROR, strerror(errno));
//         my_exit("bind failed", EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0)
//         my_exit("listen error", EXIT_FAILURE);
//     addrlen = sizeof(address);
//     _clients.insert(std::pair<int, Client*>(server_fd, NULL));
//     _creation_date = set_time();
// }

void print_client_list(std::map<int, Client*> client)
{
    for (std::map<int, Client*>::iterator it = client.begin(); it != client.end(); it++) {
        if (it->second)
            std::cout << "USER=" << it->second->GetNickname() << std::endl;
    }
}

IRCServer::IRCServer(void)
{
}

IRCServer::~IRCServer(void)
{
}

void		IRCServer::stopServer()
{
        return;
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
                accept_connection();
            else
                read_data(i);
        }
    }
    return 0;
}

// int     IRCServer::run(void)
// {
//     FD_ZERO(&readfds);
//     FD_ZERO(&all_sockets);
//     FD_SET(server_fd, &all_sockets);
//     max_sd = server_fd;
//     log(INFO, "IRC Server loop is starting");
//     while (true) {
//         readfds = all_sockets;
//         timeout.tv_sec = 0;
//         timeout.tv_usec = 0;
//         activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
//         if ((activity < 0) && (errno != EINTR))
//             my_exit("select error", EXIT_FAILURE);
//         if (activity == 0)
//         {
//             if (stop == true)
//             {
//                 close(server_fd);
//                 return 0;
//             }
//             continue;
//         }
//         print_client_list(this->_clients);
//         log(INFO, "Server new socket activity");
//         int i = 0;
// 	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
//             if (FD_ISSET(it->first, &readfds) == 1) // check if any fd i have data waiting 
//             {
//                 i = (it->first);
//                 break;
//             }    
//         }
//         log(DEBUG, "Server Ready");
//         if (i == server_fd)
//             accept_connection(&all_sockets);
//         else
//             read_data(i);
//     }
//     return 0;
// }

void    IRCServer::read_data(int i)
{
    int client_fd = events[i].data.fd;
    Client* client = _clients.find(client_fd)->second;
    log(INFO, "Clear buffer");
    bzero(client->GetBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((valread = recv(client_fd, client->GetBuffer(), 1024, 0)) <= 0) {
        remove_client(client);
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
            close(client_fd);
            clientPartialBuffers.erase(client_fd);
            return;
        }

        std::string remainingPartial;
        log(DEBUG, "Spliting buffer=" + completeBuffer );
        std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);

        clientPartialBuffers[client_fd] = remainingPartial;
        log(DEBUG, "Loop director");
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
            _director->parseCommand(client, *it);


    }
    //processBuffer(*this, client->GetSocket(), client->GetBuffer());
}

// void    IRCServer::read_data(int i)
// {
//     Client* client = (_clients.find(i))->second;
//     log(INFO, "Clear buffer");
//     bzero(client->GetBuffer(), 1024);
//     log(INFO, "Server reading data");
//     if ((valread = recv(i, client->GetBuffer(), 1024, 0)) == 0) {
//         close(sd);
//         log(WARN, "recv: socket closed");
//         if (valread == -1)  
//             log(ERROR, "recv: error");
//     }
//     else
//     {
//         log(DEBUG, client->GetBuffer());
//         std::string& clientPartial = clientPartialBuffers[i];
//
//         std::string completeBuffer = clientPartial + client->GetBuffer();
//         if (completeBuffer.size() > MAX_BUFFER_SIZE) {
//             log(ERROR, "Buffer overflow from client , disconnecting.");
//             close(i);
//             clientPartialBuffers.erase(i);
//             return;
//         }
//
//         std::string remainingPartial;
//         log(DEBUG, "Spliting buffer=" + completeBuffer );
//         std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);
//
//         clientPartialBuffers[i] = remainingPartial;
//         log(DEBUG, "Loop director");
//         for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
//             _director->parseCommand(client, *it);
//
//
//     }
//     //processBuffer(*this, client->GetSocket(), client->GetBuffer());
// }

void    IRCServer::accept_connection()
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
    if (new_socket > max_sd)
        max_sd = new_socket;
    _clients.insert(std::pair<int, Client*>(new_socket, new Client(new_socket, address, getIRCServer())));
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

IRCServer*	IRCServer::getIRCServer()
{
    return  this;
}

std::string	IRCServer::getPort()
{
    return _port_string;
}

bool    IRCServer::getpasswordIsSet()
{
    return _passwordIsSet;
}

void    IRCServer::sendReply(int target, std::string message)
{
	log(DEBUG, "REPLY SERVER :" + message);
	send(target, message.c_str(), message.size(), 0);
}

Channel *IRCServer::create_channel(std::string channel, Client *client, std::string key)
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

Client	*IRCServer::findClient(std::string nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second && it->second->GetNickname() == nickname)
            return it->second;
    }
    return (NULL);
}

void	IRCServer::remove_client(Client *client)
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

void	IRCServer::remove_channel(Channel *channel)
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

void    IRCServer::setCommandTemplate()
{
    _director = new CommandDirector(); 

    _director->addCommand(TemplateBuilder::Builder()
                          .name("CAP")
                          .level(NONE)
                          .param("toto", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new CapCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("PASS")
                          .level(NONE)
                          .param("password", ParamTemplate::Builder()
                                 .addChecker(&isConnected)
                                 .addChecker(&isValidPassword)
                                 .build()
                                 )
                          .command(new PassCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("NICK")
                          .level(CONNECTED)
                          .param("nick", ParamTemplate::Builder()
                                 .addChecker(&isEmpty) 
                                 .addChecker(&isValidNick)
                                 .build()
                                 )
                          .command(new NickCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("USER")
                          .level(CONNECTED)
                          .param("user", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("mode", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("unused", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("realname", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new UserCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("JOIN")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&isValidChannel)
                                 .addChecker(&isInLimits)
                                 .addChecker(&isInvited)
                                 .build()
                                 )
                          .param("key", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new JoinCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("PART")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("message", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new PartCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("QUIT")
                          .level(REGISTERED)
                          .trailing("message",  ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new QuitCommand())
                          .build()
                          );
    
    _director->addCommand(TemplateBuilder::Builder()
                          .name("MODE")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .build()
                                 )
                          .param("modes",  ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new ModeCommand())
                          .parser(new ModeCommandParser())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("TOPIC")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .addChecker(&isTmodeOn)
                                 .build()
                                 )
                          .trailing("topic", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new TopicCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("INVITE")
                          .level(REGISTERED)
                          .param("nickname", ParamTemplate::Builder()
                                 .addChecker(&nickExist)
                                 .build()
                                 )
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&isValidInvite)
                                 .build()
                                 )
                          .command(new InviteCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("KICK")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("user", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("comment", ParamTemplate::Builder()
                                 .build()
                                 )

                          .command(new KickCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("PRIVMSG")
                          .level(REGISTERED)
                          .param("msgtarget", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("message",  ParamTemplate::Builder()
                                 .build()
                                 )

                          .command(new PrivmsgCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("WHO")
                          .level(REGISTERED)
                          .param("mask", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .param("o", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new WhoCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("WHOIS")
                          .level(REGISTERED)
                          .param("target", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("mask", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new WhoisCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("PING")
                          .level(REGISTERED)
                          .param("server1", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("server2", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new PingCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("PONG")
                          .level(REGISTERED)
                          .param("server1", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("server2", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new PongCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("NAMES")
                          .level(REGISTERED)
                          .param("channels", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new NamesCommand())
                          .build()
                          );
}

bool	IRCServer::checkNick(const std::string& nick)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second && it->second->GetNickname() == nick)
                    return false;
	}
    return true;
}

std::string	IRCServer::set_time()
{
    time_t timestamp = time( NULL );
    struct tm * pTime = localtime( & timestamp );

    char buffer[80];
    strftime( buffer, 80, "%d/%m/%Y %H:%M:%S", pTime );
    return (buffer);
}
