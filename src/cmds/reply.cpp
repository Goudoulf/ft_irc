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

#include "../../includes/ircserv.h"
#include "../Client.hpp"

void	reply_server(std::string command,  Client &client, std::string message)
{
	int sd = client.GetSocket();
	std::string nickname = client.GetNickname();
	std::string rpl(":127.0.0.1 " + command + " " + client.GetNickname() + " " +  message +"\r\n");
	std::cout << "Reply = " << rpl << std::endl;
	send(sd, rpl.c_str(), rpl.length(), 0);
}

void	message_server(std::string target, std::string command, Client &client, std::string message, int sd)
{
	std::string rpl(":" + client.GetPrefix() + " " + command + " " + target + " " + message +"\r\n");
	std::cout << "Message to " << client.GetNickname() << " = " << rpl << std::endl;
	send(sd, rpl.c_str(), rpl.length(), 0);
}
