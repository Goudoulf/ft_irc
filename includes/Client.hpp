/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:10:52 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 09:54:26 by lvallini         ###   ########.fr       */
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
#include <ctime>
#include "CmdLevel.h"

class IRCServer;

class Client
{
	public:

		Client(const int &socket, struct sockaddr_in address);
		~Client();

		void		replyServer(std::string message);
		void		sendMessage(int target, std::string message);

		void		setNickname(std::string nickname);
		void		setHostname(std::string nickname);
		void		setUsername(std::string username);
		void		setLevel(CmdLevel level);
		void		setSocket(int i);
		void		setPrefix();
		void		setBuffer(const char * string);
		void		setIsConnected(bool isconnected);
		void		setIsRegistered(bool isregistered);
		void		setLastActivity(time_t newActivity);
		void		setPartialBuffer(std::string newPartialBuffer);
		void		erasePartialBuffer ();

		int			getSocket() const;
		char		*getBuffer();
		bool		getIsConnected()const;
		bool		getIsRegistered()const;
		time_t		getSignOnTime()const;
		time_t		getLastActivity()const;
		Client		*getClient();
		CmdLevel	getLevel() const;
		std::string	getPrefix() const;
		std::string	getBufferString()const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string	getHostname() const;
		std::string getPartialBuffer() const;

		bool operator <(const Client& toComp) const;

	private:

		std::string	_nickname; 
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;
		std::string _prefix;
		std::string _partialBuffer;
		time_t		_signOnTime;
		time_t		_lastActivity; //TODO: update on privmsg 
		bool		_isconnected;
		bool		_isRegistered;
		CmdLevel	_level;
		char		*_buffer;
		int			_socket;
};

// #endif
