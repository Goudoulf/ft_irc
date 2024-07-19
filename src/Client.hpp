#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		Client(std::string nickname, std::string username, std::string realname, std::string hostname, std::string server, const int &socket);
		~Client();

		void		SetNickname(std::string nickname);
		void		SetUsername(std::string username);
		void		SetIsOP();
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		int			GetSocket() const;
		void		SetSocket(int i);
		bool		GetIsOP() const;

	private:
		std::string _nickname; // 9 char and unique ; name which appears for other users
		std::string _username; // username on the host ; name you use to login, allows system to identify you
		std::string _realname; // name of the host ; just additional information about you
		std::string _hostname;
		std::string _server;   // server to which it's connected
		int			_socket;
		bool		_isOP;
};

#endif