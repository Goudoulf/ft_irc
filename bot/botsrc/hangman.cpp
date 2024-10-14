#include "../hangman.hpp"

HangMan::HangMan(std::string type, std::vector<std::string> players)
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "HangMan game created" << std::endl;
}

HangMan::~HangMan(){}

void HangMan::createRoom()
{
	std::cout << "create room" << std::endl;
}

bool HangMan::winCondition()
{
	return true;
}


void HangMan::gameLoop()
{
}
