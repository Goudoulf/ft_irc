#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		Client(const int &socket);
		~Client();

		void		SetClient();
		void		findnick(char *buffer);
		void		finduser(char *buffer);
		void		SetNickname(std::string nickname);
		void		SetHostname(std::string nickname);
		void		SetUsername(std::string username);
		void		SetSocket(int i);
		void		SetIsOP();
		void		SetBuffer(const char * string);
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		std::string	GetHostname() const;
		int			GetSocket() const;
		bool		GetIsOP() const;
		std::string	GetBuffer();
		char	*buffer;

	private:
		std::string _nickname; // 9 char and unique ; name which appears for other users
		std::string _username; // username on the host ; name you use to login, allows system to identify you
		std::string _realname; // name of the host ; just additional information about you
		std::string _hostname;
		std::string _server;   // server to which it's connected
		char		*_buffer;
		int			_socket;
		bool		_isOP; //un client n'est pas OP general, il est OP dans un channel en particulier
};

#endif
