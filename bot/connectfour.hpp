#ifndef CONNECTFOUR_HPP
# define CONNECTFOUR_HPP

# include "game.hpp"


class ConnectFour : public Game
{
	private:
		std::string	_player1;
		std::string	_player2;
		size_t		_turn;

	public:
		ConnectFour(std::string type, std::vector<std::string> players);
		~ConnectFour();

		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
		void	displayGame();
		void	gameLoop();

		bool	checkDirection(int deltaX, int deltaY);
};

#endif