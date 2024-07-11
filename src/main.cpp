/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 07:10:56 by cassie            #+#    #+#             */
/*   Updated: 2024/07/09 13:07:34 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <iostream>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <stdlib.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <cerrno>
// #define PORT 2512
//
// int	socket_setup()
// {
// 	int	server_fd, new_socket;
// 	struct sockaddr_in address;
// 	int opt = 1;
//     socklen_t addrlen = sizeof(address);
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }
//  
//     // Forcefully attaching socket to the port 8080
//     if (setsockopt(server_fd, SOL_SOCKET,
//                    SO_REUSEADDR | SO_REUSEPORT, &opt,
//                    sizeof(opt))) {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
//  
//     // Forcefully attaching socket to the port 8080
//     if (bind(server_fd, (struct sockaddr*)&address,
//              sizeof(address))
//         < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }
//     fd_set readfds;
//     FD_ZERO(&readfds);
//     FD_SET(server_fd, &readfds);
//     int max_sd = server_fd;
//     while (true) {
//         fd_set readfds_copy = readfds;
//         int activity = select(max_sd + 1, &readfds_copy, NULL, NULL, NULL);
//
//         if ((activity < 0) && (errno != EINTR)) {
//             perror("select error");
//         }
//
//         // Check for new connections
//         if (FD_ISSET(server_fd, &readfds_copy)) {
//             int new_socket;
//             if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
//             {
//                 perror("accept");
//                 exit(EXIT_FAILURE);
//             }
//
//             // Add new socket to set
//             FD_SET(new_socket, &readfds);
//             if (new_socket > max_sd) {
//                 max_sd = new_socket;
//             }
//         }
//
//         // Check for I/O operations on existing clients
//         for (int i = 0; i <= max_sd; i++) {
//             if (FD_ISSET(i, &readfds_copy)) {
//                 // Handle incoming data
//                 char buffer[1024] = {0};
//                 int valread = read(i, buffer, 1024);
//                 if (valread == 0) {
//                     // Client disconnected, close socket and remove from set
//                     close(i);
//                     FD_CLR(i, &readfds);
//                 } else {
//                     // Process client data
//                     buffer[valread] = '\0';
//                     printf("Received message: %s\n", buffer);
//                     // Echo the message back to the client
//                     send(i, buffer, strlen(buffer), 0);
//                 }
//             }
//         }
//     }
//     // closing the connected socket
//     close(new_socket);
//     // closing the listening socket
//     close(server_fd);
//     return 0;
// }
//
// int	main(int argc, char **argv)
// {
//         (void)argv;
// 	if (argc != 3)
// 	{
// 		std::cout << "Wrong number of arguments" << std::endl;
// 		return (-1);
// 	}
//         socket_setup();
// 	return (0);
// }
#include <iostream>
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

#define PORT 6667
#define MAX_CLIENTS 30

int main() {
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
    address.sin_port = htons(PORT);
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
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}
