/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/10/16 14:48:37 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "CommandDirector.hpp"
#include "TemplateBuilder.hpp"
#include <netinet/in.h>
#include <cerrno>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "cmds.h"
#include "CmdLevel.h"
#define MAX_EVENTS 10
#define MAX_CLIENTS 30

class Client;
class Channel;
class CommandDirector;
class TemplateBuilder;

class IRCServer
{
	public:

		static IRCServer*	getInstance();

			
		~IRCServer();
		void					initialize(std::string port, std::string password);
		void					initSocket();
		int						run();
		void					stopServer();

		void					acceptConnection();
		void					readData(int i);

		void					sendReply(int target, std::string message);
		Channel					*createChannel(std::string channel, Client *client, std::string key);
		bool					checkNick(const std::string& Nick);
		void					removeClient(Client *client);
		void					removeChannel(Channel *channel);
		Channel					*findChannel(std::string channel);
		Client					*findClient(std::string nickname);

		std::map<int, Client*>*	getClients();
		std::vector<Channel*>*	getChannels();
		std::string				getCreationDate();
		std::string				getPort();
		std::string				getPassword();
		bool					getPasswordIsSet();

	private:

		static IRCServer* _instance;

		IRCServer() {}


		int server_fd, valread, addrlen, epoll_fd;
		u_int16_t				_port;
		std::string				_port_string;
		std::string				_password;
		bool					_passwordIsSet;
		std::vector<Channel*>	_channels;
		std::string				_creation_date;
		struct sockaddr_in		address;
		struct epoll_event		event, events[MAX_EVENTS];
		std::map<int, Client*>	_clients;
		CommandDirector			*_director;
};

