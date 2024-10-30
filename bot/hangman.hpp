#ifndef HANGHIM_HPP
# define HANGHIM_HPP

# include "game.hpp"
#include <vector>
#include <algorithm>
#include <fstream>


class HangMan : public Game
{
	public:
		HangMan(std::string type, std::vector<std::string> players);
		~HangMan();

	private:
		size_t								_attempt;
		std::string							_lettersTried;
		std::string							_lettersGuessed;
		std::string							_wordToGuess;
		std::vector<std::string>			_wordsTried;
		std::vector<std::string>::iterator	_itPlayer;

		void	setNewWordToGuess();

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
};

#endif