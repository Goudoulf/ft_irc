#include "Client.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include "../includes/debug.h"
#include "IRCServer.hpp"

Client::Client(const int &socket, std::string hostname, IRCServer* server): _hostname(hostname), _socket(socket)
{
	_buffer = new char[1024];
	//buffer = new char[1024];
	_nickname = "default";
	_realname = "realname";
	_isconnected = false;
	_server = server;
}

Client::~Client()
{}

bool Client::operator<(const Client &toComp) const
{
	return _socket < toComp._socket;
}

void	Client::findnick(const char *buffer)
{
	char *temp = new char[1024];
	char *p;
	temp = strcpy(temp, buffer);
	p = strtok(temp, "  \r\n");
	while (p != NULL)
	{
		if (strcmp(p, "NICK") == 0)
		{
			p = strtok(NULL , " \r\n");
			// std::cout << "p=" << p << std::endl;
			_nickname = p;
			break ;
		}

		p = strtok(NULL , " \r\n");
	}
	log(DEBUG, "New user nick = " + _nickname);
}

void	Client::finduser(const char * buffer)
{
	char *temp = new char[1024];
	char *p;
	temp = strcpy(temp, buffer);
	p = strtok(temp, " \r\n");
	while (p != NULL)
	{
		if (strcmp(p, "USER") == 0)
		{
			p = strtok(NULL , " \r\n");
			_username = p;
			break ;
		}

		p = strtok(NULL , " \r\n");
	}
	log(DEBUG, "New user username = " + _username);
}

void		Client::SetPrefix()
{
	_prefix = _nickname + "!" + _username + "@" + _hostname;	
}

void	Client::SetClient()
{
}

void	Client::SetNickname(std::string nickname)
{
	_nickname = nickname;
}

void	Client::SetHostname(std::string hostname)
{
	_hostname = hostname;
}

void	Client::SetUsername(std::string username)
{
	_username = username;
}

void	Client::SetSocket(int i)
{
	_socket = i;
}

void	Client::SetBuffer(const char * string)
{
	_buffer = strcpy(_buffer, string);
}

void		Client::SetIsConnected(bool isconnected)
{
	_isconnected = isconnected;
}

std::string	Client::GetNickname() const
{
	return _nickname;
}

std::string	Client::GetHostname() const
{
	return _hostname;
}

std::string	Client::GetUsername() const
{
	return _username;
}

std::string	Client::GetRealname() const
{
	return _realname;
}

IRCServer*	Client::getServer()const
{
	return _server;
}

int	Client::GetSocket() const
{
	return _socket;
}

char	*Client::GetBuffer()
{
	return _buffer;
}

std::string	Client::GetPrefix() const
{
	return _prefix;
}

std::string	Client::GetBufferString()
{
	return _buffer;
}

bool		Client::GetIsConnected()
{
	return _isconnected;
}

Client		*Client::GetClient()
{
	return this;
}
