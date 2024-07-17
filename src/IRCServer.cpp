/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjacq <rjacq@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:21:58 by cassie            #+#    #+#             */
/*   Updated: 2024/07/17 10:27:41 by rjacq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include <string>
#include <iostream>

IRCServer::IRCServer(std::string port, std::string password)
{
	char *end;
	_port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
	_password = password;
}

IRCServer::~IRCServer(void)
{}

int	IRCServer::run(void)
{
	int server_fd, new_socket, client_socket[MAX_CLIENTS] = {0}, max_sd, sd, activity, valread;
	struct sockaddr_in address;
	fd_set readfds;
	char buffer[1024];

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int addrlen = sizeof(address);
	while (true) {
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		max_sd = server_fd;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			sd = client_socket[i];
			if (sd > 0) FD_SET(sd, &readfds);
			if (sd > max_sd) max_sd = sd;
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			perror("select error");
		}

		if (FD_ISSET(server_fd, &readfds)) {
			if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					break;
				}
			}
		}

		for (int i = 0; i < MAX_CLIENTS; i++) {
			sd = client_socket[i];
			if (FD_ISSET(sd, &readfds)) {
				if ((valread = read(sd, buffer, 1024)) == 0) {
					close(sd);
					client_socket[i] = 0;
				} else {
					buffer[valread] = '\0';
					std::cout << buffer;
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	return 0;
}
