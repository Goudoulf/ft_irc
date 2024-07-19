#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		Client(const int &socket);
		~Client();

		void		SetClient();
		void		findnick(std::string buffer);
		void		finduser(std::string buffer);
		void		SetNickname(std::string nickname);
		void		SetUsername(std::string username);
		void		SetSocket(int i);
		void		SetIsOP();
		void		SetBuffer(char * string);
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		int			GetSocket() const;
		bool		GetIsOP() const;
		std::string	GetBuffer();

	private:
		std::string _nickname; // 9 char and unique ; name which appears for other users
		std::string _username; // username on the host ; name you use to login, allows system to identify you
		std::string _realname; // name of the host ; just additional information about you
		std::string _hostname;
		std::string _server;   // server to which it's connected
		std::string _buffer;
		int			_socket;
		bool		_isOP;
};

#endif
