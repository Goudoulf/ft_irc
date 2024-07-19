/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/07/15 08:39:33 by cassie           ###   ########.fr       */
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
#define MAX_CLIENTS 30

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
		
		int	run(void);
		int	join(void);

	private:

		u_int16_t _port;
		std::string _password;
		unsigned	short	_client_count;
		std::map<std::string, Client> _clients;
		std::map<std::string, Client>::iterator _it;
		int server_fd, new_socket, max_sd, sd, activity, valread;
		struct sockaddr_in address;
		fd_set readfds;
		int addrlen;
		char buffer[1024];
		//std::map<std::string, Channel> _channel;

		IRCServer(void);
		IRCServer(IRCServer&);
		IRCServer& operator=(IRCServer&);
};

#endif
