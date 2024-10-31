/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:26:00 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 11:08:25 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandDirector.hpp"

#include <map>
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define MAX_CLIENTS 30

class Client;
class Channel;
class CommandDirector;

class IRCServer
{
	public:

		static IRCServer*	getInstance();
			
		~IRCServer();
		void					initialize(std::string port, std::string password);
		bool					initSocket();
		int						run();

		bool					acceptConnection();
		bool					readData(int i);

		void					sendReply(int target, std::string message);
		Channel					*createChannel(std::string channel, Client *client, std::string key);
		bool					checkNick(const std::string& Nick);
		void					removeClient(Client *client);
		void					removeAllClient();
		void					removeChannel(Channel *channel);
		Channel					*findChannel(std::string channel);
		Client					*findClient(std::string nickname);

		std::map<int, Client*>*	getClients();
		std::map<std::string, Channel*>*	getChannels();
		std::string				getCreationDate();
		std::string				getPort();
		std::string				getPassword();
		bool					getPasswordIsSet();
		int*					getPipeFd();

	private:

		IRCServer() {}

		static IRCServer*				_instance;

		u_int16_t						_port;
		std::string						_portString;
		std::string						_password;
		bool							_passwordIsSet;
		std::string						_creation_date;
		struct sockaddr_in				_address;
		struct epoll_event				_event, _events[MAX_EVENTS];
		std::map<std::string, Channel*>	_channels;
		std::map<int, Client*>			_clients;
		CommandDirector*				_director;
		int _serverfd, _valread, _addrlen, _epollfd;
		int *_pipefd;
};
#endif
