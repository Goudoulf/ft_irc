/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/07/12 10:53:10 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

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

typedef struct s_data
{
	int					server_fd;
	int					socket;
	int					client_socket[MAX_CLIENTS];
	int					max_sd;
	int					sd;
	int					activity;
	int					valread;
	fd_set				readfds;
	char				buffer[1024];
}	t_data;

class Socket
{
	public:
			
		Socket(void);
		~Socket(void);
		Socket(Socket&);
		Socket& operator=(Socket&);

	private:
		
		t_data				*_socket_data;
		struct sockaddr_in	*_address;
};

#endif
