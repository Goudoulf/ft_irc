/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:10:52 by cassie            #+#    #+#             */
/*   Updated: 2024/09/06 10:11:00 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "IRCServer.hpp"
#include <map>
#include <string>

class Client
{
	public:
		Client(const int &socket, std::string hostname);
		~Client();
		bool operator <(const Client& toComp) const;

		void		SetClient();
		void		findnick(const char *buffer);
		void		finduser(const char *buffer);
		void		SetNickname(std::string nickname);
		void		SetHostname(std::string nickname);
		void		SetUsername(std::string username);
		void		SetSocket(int i);
		void		SetPrefix();
		void		SetBuffer(const char * string);
		void		SetIsConnected(bool isconnected);
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		std::string	GetHostname() const;
		Client		*GetClient();
		int			GetSocket() const;
		std::string	GetPrefix() const;
		char		*GetBuffer();
		std::string	GetBufferString();
		bool		GetIsConnected();

	private:
		std::string	_nickname; // 9 char and unique ; name which appears for other users
		std::string	_username; // username on the host ; name you use to login, allows system to identify you
		std::string	_realname; // name of the host ; just additional information about you
		std::string	_hostname;
		std::string _prefix;
		bool		_isconnected;
		//IRCServer	*_server;   // server to which it's connected
		char		*_buffer;
		int			_socket;
		std::map<int, std::string> _clientPartialBuffers;
};

#endif
