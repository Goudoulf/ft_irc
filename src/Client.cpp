#include "Client.hpp"

Client::Client(std::string nickname, std::string username, std::string realname, std::string server, const int &socket):
	_nickname(nickname), _username(username), _realname(realname), _server(server), _socket(socket)
{
	_isOP = false;
}

Client::~Client()
{}

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
	_nickname += '@';
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

bool	Client::GetIsOP() const
{
	return _isOP;
}
