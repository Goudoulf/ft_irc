#ifndef CONNECTFOUR_HPP
# define CONNECTFOUR_HPP

# include "game.hpp"


class ConnectFour : public Game
{
	private:
		size_t	_turn;

	public:
		ConnectFour(std::string type, std::vector<std::string> players);
		~ConnectFour();
		void createRoom();

		void	resetGameState();
		void	displayGame();
		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		void	gameLoop();

		std::string*	getGameState() const;
};

#endif