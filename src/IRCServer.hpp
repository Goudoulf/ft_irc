/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/08/02 15:53:53 by rjacq            ###   ########.fr       */
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
#include "test/header/CommandDirector.hpp"
#include "test/header/TemplateBuilder.hpp"
#include <netinet/in.h>
#include <cerrno>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include "../includes/cmds.h"
#define MAX_CLIENTS 30

class Client;
class Channel;
class CommandDirector;
class TemplateBuilder;

class IRCServer
{
	public:
			
		IRCServer(std::string port, std::string password);
		~IRCServer(void);

		class InvalidPort: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidPassword: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		std::map<int, Client*> *getClients();
		std::vector<Channel*> *getChannels();
		std::string	getCreationDate();
		std::string	getPort();
		std::string	getPassword();
		void    setCommandTemplate();
		IRCServer*	getIRCServer();
		void		stopServer();
		
		int	run(void);
		int	join(void);
		void    accept_connection(fd_set *all_sockets);
		void    read_data(int i);
		Channel	*create_channel(std::string channel, Client &client, std::string key);
		Channel	*find_channel(std::string channel);
		void	remove_client(Client &client);
		std::string	set_time();
		struct timeval timeout;

	private:

    fd_set readfds;
    fd_set all_sockets;
		u_int16_t _port;
		std::string	_port_string;
		std::string _password;
		unsigned	short	_client_count;
		std::vector<Channel*> _channels;
		std::vector<int> _fds;
		std::vector<Client*>::iterator _it;
		std::vector<Client*>::iterator _it2;
		std::string	_creation_date;
		int server_fd, max_sd, sd, activity, valread;
		struct sockaddr_in address;
		int addrlen;
		char buffer[1024];
		std::map<int, Client*> _clients;
		CommandDirector *_director;

		IRCServer(void);
		IRCServer(IRCServer&);
		IRCServer& operator=(IRCServer&);
};
