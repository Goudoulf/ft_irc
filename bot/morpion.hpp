#ifndef MORPION_HPP
#define MORPION_HPP

#include "game.hpp"

class Morpion: public Game
{
	public:
		Morpion(std::string type, std::vector<std::string> players);
		~Morpion();

		void	gameLoop();

	private:
		int			_x;
		int			_y;
		size_t		_turn;
		std::string	_player1;
		std::string	_player2;

		bool	isGameReady();
		bool	isPlayerTurn();
		bool	isInputValid();

		void	updateGameState();
		void	displayGame();

		bool	checkGameOver();
		bool	handleStartCommand();
		void	initializePlayers();
		void	prepareGameStartMessage();

		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
};

#endif