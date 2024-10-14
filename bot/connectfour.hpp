#ifndef CONNECTFOUR_HPP
# define CONNECTFOUR_HPP

# include "game.hpp"


class ConnectFour : public Game
{
	public:
		ConnectFour(std::string type, std::vector<std::string> players);
		~ConnectFour();
		void createRoom();
};

#endif