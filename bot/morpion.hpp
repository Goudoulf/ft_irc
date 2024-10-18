#ifndef MORPION_HPP
#define MORPION_HPP

#include "game.hpp"

class Morpion: public Game
{
	public:
		Morpion(std::string type, std::vector<std::string> players);
		~Morpion();
		void createRoom();

		void	resetGameState();
		void	displayGame();
		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
		void	cleanBuffer();
		void	gameLoop();

	private:
		size_t		_turn;
		int			_x;
		int			_y;

};

#endif