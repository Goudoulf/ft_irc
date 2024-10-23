#ifndef MORPION_HPP
#define MORPION_HPP

#include "game.hpp"

class Morpion: public Game
{
	public:
		Morpion(std::string type, std::vector<std::string> players);
		~Morpion();
		void createRoom();

		void	displayGame();
		bool	checkInput();
		bool	winCondition();
		bool	isBufferFull();
		bool	checkStart();
		void	gameLoop();

	private:
		size_t		_turn;
		int			_x;
		int			_y;
		std::string _player1;
		std::string _player2;

};

#endif