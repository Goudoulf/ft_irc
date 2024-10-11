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
		int				getX() const;
		int				getY() const;

		void			setTurn(size_t	turn);
		void			setGameState(std::string *_gameState);
		void			setX(int x);
		void			setY(int y);

	private:
		size_t		_turn;
		std::string	*_gameState;
		int			_x;
		int			_y;

};

#endif