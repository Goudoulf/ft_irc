#include "Client.hpp"
#include "client_checker.h"

Client::Client(const int &socket): _socket(socket)
{
	_isOP = false;
}

Client::~Client()
{}

void	Client::SetClient(std::string nickname, std::string username, std::string realname, std::string hostname, std::string server)
{
	if (nickname.length() > 9)
		nickname.erase(9, -1);
	_nickname = nickname;
	_username = username;
	_realname = realname;
	_hostname = hostname;
	_server = server;
}

void	Client::SetNickname(std::string nickname)
{
	_nickname = nickname;
}

void	Client::SetUsername(std::string username)
{
	_username = username;
}

void	Client::SetHostname(std::string hostname)
{
	_hostname = hostname;
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

void	Client::SetBuffer(char * string)
{
	_buffer = string;
}

std::string	Client::GetNickname() const
{
	return _nickname;
}

std::string	Client::GetUsername() const
{
	return _username;
}

std::string	Client::GetRealname() const
{
	return _realname;
}

std::string Client::GetHostname() const
{
	return _hostname;
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
