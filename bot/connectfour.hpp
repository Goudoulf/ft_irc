#ifndef CONNECTFOUR_HPP
# define CONNECTFOUR_HPP

# include "game.hpp"


class ConnectFour : public Game
{
	private:
		std::string	_player1;
		std::string	_player2;
		size_t		_turn;

		bool	isGameReady();
		bool	isPlayerTurn();
		bool	isInputValid();
		bool	isBufferFull();

		bool	checkInput();
		bool	winCondition();
		bool	checkStart();
		bool	checkGameOver();
		bool	handleStartCommand();

		void	initializePlayers();
		void	prepareGameStartMessage();
		void	updateGameState();
		void	displayGame();

		bool	checkDirection(int deltaX, int deltaY);

	public:
		ConnectFour(std::string type, std::vector<std::string> players);
		~ConnectFour();
};

#endif