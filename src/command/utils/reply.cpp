/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 07:56:09 by cassie            #+#    #+#             */
/*   Updated: 2024/10/30 11:16:49 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.h"
#include "Client.hpp"
#include "debug.h"

void	messageServer(std::string target, std::string command, Client &client, std::string message, int sd)
{
	std::string rpl(":" + client.getPrefix() + " " + command + " " + target + " " + message +"\r\n");
	log(REPLY, rpl);
	send(sd, rpl.c_str(), rpl.length(), MSG_NOSIGNAL);
}

void	rplSend(int fd, std::string rpl)
{
	log(DEBUG, "RPL_SERVER :" + rpl);
	send(fd, rpl.c_str(), rpl.size(), 0);
}
