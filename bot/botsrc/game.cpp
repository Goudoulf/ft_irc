#include "../game.hpp"

Game::Game()
{
	_start = false;
	_finished = false;
	_error = false;
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
void Game::gameLoop()
{
	if (_error || !isGameReady() || !isPlayerTurn() || !isInputValid())
		return;

	updateGameState();
	displayGame();
}

bool Game::isFinished()
{
	return _finished;
}

bool Game::isStarted()
{
	return _start;
}

void Game::cleanBuffer()
{
	_buffer.clear();
}

void Game::setInput(std::string input)
{
	_input = input;
}

void Game::setCurrentPlayer(std::string client)
{
	_currentPlayer = client;
}

std::string Game::getBuffer() const
{
	return _buffer;
}

std::string Game::getChanName() const
{
	return _chanName;
}

std::vector<std::string> Game::getPlayers() const
{
	return _players;
}
