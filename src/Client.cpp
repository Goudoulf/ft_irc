#include "Client.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include "cmds.h"
#include "debug.h"
#include "IRCServer.hpp"

Client::Client(const int &socket, struct sockaddr_in address, IRCServer* server): _socket(socket)
{
	char test[1024];
	_buffer = new char[1024];
	_nickname = "default";
	_realname = "realname";
	if (server->getpasswordIsSet())
		_level = NONE;
	else
		_level = CONNECTED;
	_server = server;
	getnameinfo((sockaddr*)&address, sizeof(address), test, 1024, NULL, 0, 0);
	_hostname = test;
	log(INFO, "CLient creation " + _hostname);
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

void		Client::SetLevel(CmdLevel level)
{
	_level = level;	
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

void		Client::SetIsRegistered(bool isregistered)
{
	_isRegistered = isregistered;
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

CmdLevel	Client::GetLevel() const
{
	return _level;
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

bool		Client::GetIsRegistered()
{
	return _isRegistered;
}

Client		*Client::GetClient()
{
	return this;
}
