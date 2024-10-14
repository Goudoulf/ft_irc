#include "../game.hpp"

Game::Game(){}

// Game::Game(std::vector<std::string> params) {}

// Game::Game(const Game &myGame) {}

std::string Game::getChanName()
{
	return _chanName;
}


std::vector<std::string> Game::getPlayers()
{
	return _players;
}

void Game::setPlayers(std::vector<std::string> players) 
{
	_players = players;
}

std::string Game::generateChanId()
{
	time_t timestamp = time(NULL);
	std::string base36("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	std::string res;

	for (int i = 5; i > 0; i--)
	{
		res = base36.at(timestamp % 36) + res;
		timestamp /= 36;
	}
	return (res);
}
