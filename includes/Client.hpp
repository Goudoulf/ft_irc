/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:10:52 by cassie            #+#    #+#             */
/*   Updated: 2024/10/21 11:06:43 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
// #ifndef CLIENT_HPP
// #define CLIENT_HPP

#include <string>
#include "IRCServer.hpp"
#include "cmds.h"
#include <map>
#include <string>
#include "CmdLevel.h"

class IRCServer;

class Client
{
	public:
		Client(const int &socket, struct sockaddr_in address, IRCServer* server);
		~Client();
		bool operator <(const Client& toComp) const;

		void		replyServer(std::string message);
		void		sendMessage(int target, std::string message);
		void		SetClient();
		void		findnick(const char *buffer);
		void		finduser(const char *buffer);
		void		SetNickname(std::string nickname);
		void		SetHostname(std::string nickname);
		void		SetUsername(std::string username);
		void		SetLevel(CmdLevel level);
		void		SetSocket(int i);
		void		SetPrefix();
		void		SetBuffer(const char * string);
		void		SetIsConnected(bool isconnected);
		void		SetIsRegistered(bool isregistered);
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		std::string	GetHostname() const;
		CmdLevel	GetLevel() const;
		IRCServer*	getServer()const;
		Client		*GetClient();
		int			GetSocket() const;
		std::string	GetPrefix() const;
		char		*GetBuffer();
		std::string	GetBufferString();
		bool		GetIsConnected();
		bool		GetIsRegistered();
		time_t		getSignOnTime();
		time_t		getLastActivity();
		void		setLastActivity(time_t newActivity);

	private:
		std::string	_nickname; // 9 char and unique ; name which appears for other users
		std::string	_username; // username on the host ; name you use to login, allows system to identify you
		std::string	_realname; // name of the host ; just additional information about you
		std::string	_hostname;
		std::string _prefix;
		time_t		_signOnTime;
		time_t		_lastActivity;
		bool		_isconnected;
		bool		_isRegistered;
		CmdLevel	_level;
		char		*_buffer;
		int			_socket;
		std::map<int, std::string> _clientPartialBuffers;
		IRCServer*	_server;
};

// #endif
