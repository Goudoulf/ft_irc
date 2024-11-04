#include "../morpion.hpp"
#include <iostream>

Morpion::Morpion(std::string type, std::vector<std::string> players) : Game()
{
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "Morpion game created" << std::endl;
	_x = 0;
	_y = 0;
	_turn = 0;
	_gameState = new std::string[3];
	for (int i = 0; i < 3; i++)
		_gameState[i] = "   ";
}

Morpion::~Morpion()
{
	delete[] _gameState;
}

bool Morpion::isGameReady()
{
	if (!checkStart() || isBufferFull())
		return false;
	return true;
}

bool Morpion::isPlayerTurn()
{
	if ((_turn % 2 && _currentPlayer != _player2) ||
		(!(_turn % 2) && _currentPlayer != _player1))
		return false;
	return true;
}

bool Morpion::isInputValid()
{
	if (!checkInput())
		return false;
	_x = _input[0] - '0' - 1;
	_y = _input[1] - 'A';
	
	if (_gameState[_x][_y] != ' ')
	{
		_buffer += "Coordinate are already used\nPut coordinates (ex: 1A, 3B, ...):";
		return false;
	}
	return true;
}

void Morpion::updateGameState()
{
	_gameState[_x][_y] = (_turn % 2) ? 'x' : 'o';
	_turn++;
	_input.clear();
}

void Morpion::displayGame()
{
	_buffer += "\n   A   B   C\n";
	for (int i = 0; i < 3; i++)
	{
		_buffer += (char)(i + 1 + '0');
		_buffer += " ";
		for (int j = 0; j < 3; j++)
		{
			_buffer += " ";
			_buffer += _gameState[i][j];
			if (j < 2)
				_buffer += " |";
			else
				_buffer += " ";
		}
		_buffer += "\n";
		if (i < 2)
			_buffer += "  ---|---|---\n";
	}
	if (checkGameOver())
		return;
	std::string player = (_turn % 2) ? _player1 : _player2;
	std::string colorCode = (_turn % 2) ? "\x03" "4" : "\x03" "3";
	_buffer += "\n" + colorCode + player + "'s turn\nPut coordinates (ex: 1A, 3B, ...):";
}

bool Morpion::checkStart()
{
	if (_start)
		return false;
	if (_input == "!start")
		return handleStartCommand();

	_buffer.clear();
	return true;
}

bool Morpion::isBufferFull()
{
	if (checkGameOver())
		return true;

	if (_input.empty())
	{
		std::string player = (_turn % 2) ? _player1 : _player2;
		std::string colorCode = (_turn % 2) ? "\x03" "4" : "\x03" "3";
		_buffer += "\n" + colorCode + player + "'s turn\nPut coordinates (ex: 1A, 3B, ...):";
		return true;
	}

	return !checkInput();
}

bool Morpion::checkInput()
{
	if (_input.length() != 2 || 
		_input[0] < '1' || _input[0] > '3' || 
		_input[1] < 'A' || _input[1] > 'C')
	{
		_buffer += "Wrong input\nPut coordinates (ex: 1A, 3B, ...):";
		return false;
	}
	return true;
}

bool Morpion::handleStartCommand()
{
	if (_players.size() < 2)
	{
		_buffer = "Not enough player, please wait for another player!";
		return false;
	}

	initializePlayers();
	prepareGameStartMessage();
	_start = true;
	return true;
}

bool Morpion::checkGameOver()
{
	if (winCondition() || _turn >= 9)
	{
		if (_turn >= 9)
			_buffer += "Draw!";
		else
			_buffer += (_turn % 2 ? _player1 : _player2) + " wins!";
		_finished = true;
		return true;
	}
	return false;
}

void Morpion::prepareGameStartMessage()
{
	_buffer = "---------------\n|   MORPION   |\n---------------\n";
	displayGame();
}

void Morpion::initializePlayers()
{
	_player1 = _players[0];
	_player2 = _players[1];
}

bool Morpion::winCondition()
{
	for (int i = 0; i < 3; i++)
	{
		if (_gameState[i][0] == _gameState[i][1] && _gameState[i][0] == _gameState[i][2] && _gameState[i][0] != ' ')
			return true;
		else if (_gameState[0][i] == _gameState[1][i] && _gameState[0][i] == _gameState[2][i] && _gameState[0][i] != ' ')
			return true;
	}
	if (_gameState[0][0] == _gameState[1][1] && _gameState[0][0] == _gameState[2][2] && _gameState[0][0] != ' ')
		return true;
	if (_gameState[0][2] == _gameState[1][1] && _gameState[0][2] == _gameState[2][0] && _gameState[0][2] != ' ')
		return true;
	return false;
}
