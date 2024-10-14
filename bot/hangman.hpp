#ifndef HANGHIM_HPP
# define HANGHIM_HPP

# include "game.hpp"

class HangMan : public Game
{
	public:
		HangMan(std::string type, std::vector<std::string> players);
		~HangMan();
		void createRoom();
		// HangMan(const HangMan& myHangMan);
		// const HangMan& operator=(const HangMan& myHangMan);
		// void newTurn();
};

#endif