/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/07/30 16:12:14 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <map>
#include "Client.hpp"
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#define MAX_CLIENTS 30

class Client;

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

		std::vector<Client*> *getClients();
		
		int	run(void);
		int	join(void);
		void    accept_connection(fd_set *all_sockets);
		void    read_data(fd_set *all_sockets, int i);
		struct timeval timeout;

	private:

		u_int16_t _port;
		std::string _password;
		unsigned	short	_client_count;
		std::vector<Client*> _clients;
		std::vector<Client*>::iterator _it;
		std::vector<Client*>::iterator _it2;
		int server_fd, max_sd, sd, activity, valread;
		struct sockaddr_in address;
		int addrlen;
		char buffer[1024];
		//std::map<std::string, Channel> _channel;

		IRCServer(void);
		IRCServer(IRCServer&);
		IRCServer& operator=(IRCServer&);
};

#endif
