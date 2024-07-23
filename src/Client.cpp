#include "Client.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

Client::Client(const int &socket): _socket(socket)
{
	_isOP = false;
	_buffer = new char[1024];
	buffer = new char[1024];
}

Client::~Client()
{}

void	Client::findnick(char *buffer)
{
	char *temp = new char[1024];
	char *p;
	temp = strcpy(temp, buffer);
	p = strtok(temp, "  \n");
	while (p != NULL)
	{
		if (strcmp(p, "NICK") == 0)
		{
			p = strtok(NULL , " \n");
			// std::cout << "p=" << p << std::endl;
			_nickname = p;
			break ;
		}

		p = strtok(NULL , " \n");
	}
	std::cout << "nick=" << _nickname << std::endl;
}

void	Client::finduser(char * buffer)
{
	char *temp = new char[1024];
	char *p;
	temp = strcpy(temp, buffer);
	p = strtok(temp, " \n");
	while (p != NULL)
	{
		if (strcmp(p, "USER") == 0)
		{
			p = strtok(NULL , " \n");
			_username = p;
			break ;
		}

		p = strtok(NULL , " \n");
	}
	std::cout << "user=" << _username << std::endl;
}

void	Client::SetClient()
{
	findnick(_buffer);
	finduser(_buffer);
	_realname = "realname";
	_hostname = "localhost";
	_server = "unknown";
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

void	Client::SetIsOP()
{
	_isOP = true;
	_nickname.insert(0, "@");
}

void	Client::SetSocket(int i)
{
	_socket = i;
}

void	Client::SetBuffer(const char * string)
{
	_buffer = strcpy(_buffer, string);
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

int	Client::GetSocket() const
{
	return _socket;
}

bool	Client::GetIsOP() const
{
	return _isOP;
}

std::string	Client::GetBuffer()
{
	return _buffer;
}
