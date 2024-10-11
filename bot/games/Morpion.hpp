#ifndef MORPION_HPP
#define MORPION_HPP

#include "Game.hpp"

class Morpion: public Game
{
	public:
		Morpion(std::string name);
		~Morpion();

		void			resetGameState();
		void			displayGame();
		bool			checkInput();
		bool			winCondition();
		void			gameLoop();

		size_t			getTurn() const;
		std::string*	getGameState() const;

	private:
		size_t		_turn;
		std::string	*_gameState;
		int			_x;
		int			_y;

};

#endif