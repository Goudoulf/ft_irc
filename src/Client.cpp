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

Client::Client(const int &socket, struct sockaddr_in address): _socket(socket)
{
	IRCServer *server = IRCServer::getInstance();
	char temp[1024];
	_buffer = new char[1024];
	_nickname = "";
	_realname = "";
	if (server->getPasswordIsSet())
		_level = NONE;
	else
		_level = CONNECTED;
	getnameinfo((sockaddr*)&address, sizeof(address), temp, 1024, NULL, 0, 0);
	_hostname = temp;
	_signOnTime = time(NULL);
	_lastActivity = _signOnTime;
	log(INFO, "CLient creation " + _hostname);
}

Client::~Client()
{
	delete[] _buffer;
}

void    Client::replyServer(std::string message)
{
	IRCServer *server = IRCServer::getInstance();
	server->sendReply(this->_socket, message);
}

void	Client::sendMessage(int target, std::string message)
{
	log(DEBUG, "Client Reply :" + message);
	send(target, message.c_str(), message.size(), 0);
}

bool Client::operator<(const Client &toComp) const
{
	return _socket < toComp._socket;
}

void	Client::setPrefix()
{
	_prefix = _nickname + "!" + _username + "@" + _hostname;	
}

void	Client::setLevel(CmdLevel level) {_level = level;}

void	Client::setNickname(std::string nickname) {_nickname = nickname;}

void	Client::setHostname(std::string hostname) {_hostname = hostname;}

void	Client::setUsername(std::string username) {_username = username;}

void	Client::setSocket(int i) {_socket = i;}

void	Client::setBuffer(const char * string) {_buffer = strcpy(_buffer, string);}

void	Client::setIsConnected(bool isconnected) {_isconnected = isconnected;}

void	Client::setIsRegistered(bool isregistered) {_isRegistered = isregistered;}


std::string	Client::getNickname() const {return _nickname;}

std::string	Client::getHostname() const {return _hostname;}

std::string	Client::getUsername() const {return _username;}

std::string	Client::getRealname() const {return _realname;}

CmdLevel	Client::getLevel() const {return _level;}

int	Client::getSocket() const {return _socket;}

char	*Client::getBuffer() {return _buffer;}

std::string	Client::getPrefix() const {return _prefix;}

std::string	Client::getBufferString()const {return _buffer;}

bool		Client::getIsConnected()const {return _isconnected;}

bool		Client::getIsRegistered()const {return _isRegistered;}

Client		*Client::getClient() {return this;}

time_t	Client::getSignOnTime()const {return _signOnTime;}

time_t	Client::getLastActivity()const {return _lastActivity;}

void	Client::setLastActivity(time_t newActivity) {_lastActivity = newActivity;}
