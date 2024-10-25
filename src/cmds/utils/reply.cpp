/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 07:56:09 by cassie            #+#    #+#             */
/*   Updated: 2024/08/21 07:57:21 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.h"
#include "Client.hpp"
#include "debug.h"

void	reply_server(std::string command,  Client &client, std::string message)
{
	int sd = client.getSocket();
	std::string nickname = client.getNickname();
	std::string rpl(":127.0.0.1 " + command + " " + client.getNickname() + " " +  message +"\r\n");
	log(REPLY, ":127.0.0.1 " + command + " " + client.getNickname() + " " +  message);
	send(sd, rpl.c_str(), rpl.length(), 0);
}

void	message_server(std::string target, std::string command, Client &client, std::string message, int sd)
{
	std::string rpl(":" + client.getPrefix() + " " + command + " " + target + " " + message +"\r\n");
	//std::cout << "Message to " << client.getNickname() << " = " << rpl << std::endl;
	log(REPLY, rpl);
	send(sd, rpl.c_str(), rpl.length(), 0);
}

void	rpl_send(int fd, std::string rpl)
{
	log(DEBUG, "RPL_SERVER :" + rpl);
	send(fd, rpl.c_str(), rpl.size(), 0);
}
