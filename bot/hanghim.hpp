#ifndef HANGHIM_HPP
# define HANGHIM_HPP

# include "game.hpp"


class HangHim : public Game
{
	protected:
		std::string _type;
		std::string _chanName;
		std::vector<std::string> _players;

	public:
		HangHim();
		virtual ~HangHim();
		HangHim(const HangHim& myHangHim);
		const HangHim& operator=(const HangHim& myHangHim);
		void createRoom();
		void newTurn();
};

#endif