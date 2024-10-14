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
#include "TopicCommand.hpp"
#include "InviteCommand.hpp"
#include "KickCommand.hpp"
#include "PrivmsgCommand.hpp"
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


extern bool stop;
const size_t MAX_BUFFER_SIZE = 512;

std::map<int, std::string> clientPartialBuffers;

void print_client_list(std::map<int, Client*> client)
{
    for (std::map<int, Client*>::iterator it = client.begin(); it != client.end(); it++) {
        if (it->second)
            std::cout << "USER=" << it->second->GetNickname() << std::endl;
    }
}

void my_exit(std::string error, int code)
{
    log(ERROR, error);
    exit(code);
}

IRCServer::IRCServer(std::string port, std::string password)
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
    addrlen = sizeof(address);
    _clients.insert(std::pair<int, Client*>(server_fd, NULL));
    _creation_date = set_time();
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
            if (stop == true)
            {
                close(server_fd);
                return 0;
            }
            continue;
        }
        print_client_list(this->_clients);
        log(INFO, "Server new socket activity");
        int i = 0;
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
            if (FD_ISSET(it->first, &readfds) == 1) // check if any fd i have data waiting 
            {
                i = (it->first);
                break;
            }    
        }
        log(DEBUG, "Server Ready");
        if (i == server_fd)
            accept_connection(&all_sockets);
        else
            read_data(i);
    }
    return 0;
}

void    IRCServer::read_data(int i)
{
    Client* client = (_clients.find(i))->second;
    log(INFO, "Clear buffer");
    bzero(client->GetBuffer(), 1024);
    log(INFO, "Server reading data");
    if ((valread = recv(i, client->GetBuffer(), 1024, 0)) == 0) {
        close(sd);
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
            close(i);
            clientPartialBuffers.erase(i);
            return;
        }

        std::string remainingPartial;
        log(DEBUG, "Spliting buffer=" + completeBuffer );
        std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);

        clientPartialBuffers[i] = remainingPartial;
        log(DEBUG, "Loop director");
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
            _director->parseCommand(client->GetSocket(), *it, *this);


    }
    //processBuffer(*this, client->GetSocket(), client->GetBuffer());
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
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end();) {
		if (it->second && it->second->GetNickname() == client.GetNickname())
                {
                        log(DEBUG, it->second->GetNickname() + " is deleted");
                        FD_CLR(it->first, &all_sockets);
                        close(it->first);
                        delete ((it->second->GetClient()));
			it = _clients.erase(it);
                        return;
                }
		else
			++it;
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
                                 .build()
                                 )
                          .trailing("message",  ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new ModeCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("TOPIC")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .build()
                                 )
                          .param("topic", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new TopicCommand())
                          .build()
                          );

    _director->addCommand(TemplateBuilder::Builder()
                          .name("Invite")
                          .level(REGISTERED)
                          .param("nickname", ParamTemplate::Builder()
                                 .addChecker(&nickExist)
                                 .build()
                                 )
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&isOnChannel)
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
                          .trailing("comment",  ParamTemplate::Builder()
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
                                 .build()
                                 )
                          .param("o", ParamTemplate::Builder()
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
