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

		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
		void	displayGame();
		void	gameLoop();

		void	setNewWordToGuess();

	private:
		size_t								_attempt;
		std::string							_lettersTried;
		std::string							_lettersGuessed;
		std::string							_wordToGuess;
		std::vector<std::string>			_wordsTried;
		std::vector<std::string>::iterator	_itPlayer;
};

#endif