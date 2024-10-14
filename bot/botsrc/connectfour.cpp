#include "../connectfour.hpp"

ConnectFour::ConnectFour(std::string type, std::vector<std::string> players)
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "Connectfour game created" << std::endl;
}

ConnectFour::~ConnectFour(){}

void ConnectFour::createRoom()
{
	std::cout << "create room" << std::endl;
}