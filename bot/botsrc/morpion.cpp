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
	_gameState = new std::string[3] {"   ", "   ", "   "};
}

Morpion::~Morpion()
{
	delete[] _gameState;
}

void Morpion::createRoom()
{
	std::cout << "create room" << std::endl;
}

bool Morpion::checkInput()
{
	if (_input.length() != 2 || _input[0] > '3' || _input[0] < '1' || _input[1] > 'C' || _input[1] < 'A')
	{
		_buffer += "Wrong input\nPut coordinates (ex: 1A, 3B, ...):";
		return false;
	}
	return true;
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

bool Morpion::isBufferFull()
{
	if (winCondition() || _turn >= 10)
	{
		if (_turn >= 10)
			_buffer += "Draw!";
		else if (_turn % 2)
			_buffer += "Player 2 win!";
		else
			_buffer += "Player 1 win!";
		_finished = true;
		return true;
	}
	if (_input.empty())
	{
		if (_turn % 2)
			_buffer += "\n" "\x03" "4" "Player 1 turn\nPut coordinates (ex: 1A, 3B, ...):";
		else
			_buffer += "\n" "\x03" "3" "Player 2 turn\nPut coordinates (ex: 1A, 3B, ...):";
		return true;
	}
	if (!checkInput())
		return true;
	return false;
}

bool Morpion::checkStart()
{
	if (_turn == 0 && _input == "!start")
	{
		_buffer = "---------------\n|   MORPION   |\n---------------\n";
		displayGame();
		_buffer += "\n" "\x03" "4" "Player 1 turn\nPut coordinates (ex: 1A, 3B, ...):";
		_turn++;
		return true;
	}
	else if (_turn == 0)
	{
		_buffer.clear();
		return true;
	}
	return false;
}

void Morpion::gameLoop()
{
	if (checkStart())
		return;
	if (isBufferFull())
		return;
	if (!checkInput())
		return;

	_x = _input[0] - '0' - 1;
	_y = _input[1] - 'A';
	
	if (_gameState[_x][_y] != ' ')
	{
		_buffer += "Coordinate are already used\nPut coordinates (ex: 1A, 3B, ...):";
		return;
	}
	if (_turn % 2)
		_gameState[_x][_y] = 'x';
	else
		_gameState[_x][_y] = 'o';
	_turn++;

	displayGame();
	_input.clear();
	if (isBufferFull())
		return;
}
