#ifndef BOT_HPP
# define BOT_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <iostream>
# include <string.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>
# include <sstream>
# include "game.hpp"
# include "hanghim.hpp"

class Bot
{
	private:
		std::vector<Game*> _games;
		int _socketFd;
		u_int16_t _port;
		struct sockaddr_in _address;

	public:
		Bot(std::string port);
		~Bot();
		Game *findGame(std::string toFind);
		void addGame(Game *newGame);
		std::vector<Game*> getGames();
		void run();
		void readData(std::string buffer);
};

#endif