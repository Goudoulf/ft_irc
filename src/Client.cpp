#include "Client.hpp"
#include <cstddef>
#include <cstring>
#include <string>

Client::Client(const int &socket): _socket(socket)
{
	_isOP = false;
}

Client::~Client()
{}

void	Client::findnick(std::string buffer)
{
	std::size_t found = buffer.find("NICK");
	std::size_t end = buffer.find("\r", found);
	_nickname = buffer.substr(found + 5, end - 1);
}

void	Client::finduser(std::string buffer)
{
	std::size_t found = buffer.find("USER");
	std::size_t end = buffer.find(" ", found);
	_username = buffer.substr(found + 5, end - 1);
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
