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
#define MAX_EVENTS 10
#include "CmdLevel.h"
#define MAX_CLIENTS 30

class Client;
class Channel;
class CommandDirector;
class TemplateBuilder;

class IRCServer
{
	public:

		static IRCServer*	getInstance();

		void	initialize(std::string port, std::string password);
			
		~IRCServer(void);
		void					sendReply(int target, std::string message);
		void					setCommandTemplate();
		IRCServer*				getIRCServer();
		void					stopServer();
		int						run(void);
		void					accept_connection();
		void					read_data(int i);
		Channel					*create_channel(std::string channel, Client *client, std::string key);
		Channel					*find_channel(std::string channel);
		bool					checkNick(const std::string& Nick);
		void					remove_client(Client *client);
		std::map<int, Client*>	*getClients();
		std::vector<Channel*>	*getChannels();
		std::string				getCreationDate();
		std::string				getPort();
		std::string				getPassword();
		bool					getpasswordIsSet();
		std::string				set_time();
		struct timeval timeout;
		Client					*findClient(std::string nickname);

	private:

		static IRCServer* _instance;

		IRCServer();


		int server_fd, max_sd, sd, activity, valread, addrlen, epoll_fd;
		fd_set					readfds;
		fd_set					all_sockets;
		u_int16_t				_port;
		std::string				_port_string;
		std::string				_password;
		bool					_passwordIsSet;
		std::vector<Channel*>	_channels;
		std::string				_creation_date;
		unsigned short			_client_count;
		struct sockaddr_in		address;
		struct epoll_event event, events[MAX_EVENTS];
		std::map<int, Client*>	_clients;
		CommandDirector			*_director;

		IRCServer(IRCServer&);
		IRCServer& operator=(IRCServer&);
};

