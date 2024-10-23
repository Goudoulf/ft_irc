#ifndef HANGHIM_HPP
# define HANGHIM_HPP

# include "game.hpp"
#include <vector>
#include <fstream>


class HangMan : public Game
{
	public:
		HangMan(std::string type, std::vector<std::string> players);
		~HangMan();
		void createRoom();

		void	displayGame();
		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
		void	gameLoop();
		void	setNewWordToGuess();
		// HangMan(const HangMan& myHangMan);
		// const HangMan& operator=(const HangMan& myHangMan);
		// void newTurn();
	
	private:
		size_t		_attempt;
		std::string	_lettersTried;
		std::vector<std::string> _wordsTried;
		std::string	_lettersGuessed;
		std::string	_wordToGuess;
		std::vector<std::string>::iterator _itPlayer;
};

#endif