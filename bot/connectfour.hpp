#ifndef CONNECTFOUR_HPP
# define CONNECTFOUR_HPP

# include "game.hpp"


class ConnectFour : public Game
{
	protected:
		std::string _type;
		std::string _chanName;
		std::vector<std::string> _players;

	public:
		ConnectFour(std::vector<std::string> params);
		~ConnectFour();
		ConnectFour(const ConnectFour& myConnectFour);
		const ConnectFour& operator=(const ConnectFour& myConnectFour);
		void createRoom();
		void newTurn();
};

#endif