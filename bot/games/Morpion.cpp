#include "Morpion.hpp"

Morpion::Morpion(std::string name) : Game()
{
	_name = name;
	resetGameState();
	_x = 0;
	_y = 0;
	_turn = 1;
	_input = "";
	_gameState = new std::string[3] {"   ", "   ", "   "};
	setBuffer("---------------\n|   MORPION   |\n---------------");
	displayGame();
	gameLoop();
}

Morpion::~Morpion()
{
	delete[] _gameState;
}

void Morpion::resetGameState()
{
	_gameState[0] = "   ";
	_gameState[1] = "   ";
	_gameState[2] = "   ";
}

bool Morpion::checkInput()
{
	if (_input.length() != 2 || _input[0] > '3' || _input[0] < '1' || _input[1] > 'C' || _input[1] < 'A')
	{
		setBuffer(_buffer + "\nWrong input\nPut coordinates (ex: 1A, 3B, ...):");
		return false;
	}
	return true;
}

void Morpion::displayGame()
{
	setBuffer(getBuffer() + "\n   A   B   C\n");
	for (int i = 0; i < 3; i++)
	{
		setBuffer(getBuffer() + (char)(i + 1 + '0') + " ");
		for (int j = 0; j < 3; j++)
		{
			if (j < 2)
				setBuffer(getBuffer() + " " + _gameState[i][j] + " |");
			else
				setBuffer(getBuffer() + " " + _gameState[i][j] + " ");
		}
		setBuffer(getBuffer() + "\n");
		if (i < 2)
			setBuffer(getBuffer() + "  ---|---|---\n");
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

void Morpion::gameLoop()
{
	if (winCondition() || _turn >= 10)
	{
		if (_turn >= 10)
		{
			setBuffer(_buffer + "Draw!");
			return;
		}
		else if (_turn % 2)
		{
			setBuffer(_buffer + "Player 1 win!");
			return;
		}
		else
		{
			setBuffer(_buffer + "Player 2 win!");
			return;
		}
	}
	if (_input.empty())
	{
		if (_turn % 2)
			setBuffer(_buffer + "\n\e[1;31mPlayer 1 turn\e[0m\nPut coordinates (ex: 1A, 3B, ...):");
		else
			setBuffer(_buffer + "\n\e[1;32mPlayer 2 turn\e[0m\nPut coordinates (ex: 1A, 3B, ...):");
	}
	if (!checkInput())
		return;
	_x = _input[0] - '0' - 1;
	_y = _input[1] - 'A';
	if (_gameState[_x][_y] != ' ')
	{
		setBuffer(_buffer + "Coordinate are already used\nPut coordinates (ex: 1A, 3B, ...):");
		return;
	}
	if (_turn % 2)
		_gameState[_x][_y] = 'x';
	else
		_gameState[_x][_y] = 'o';
	_turn++;
	displayGame();
}

size_t Morpion::getTurn() const
{
	return _turn;
}

std::string* Morpion::getGameState() const
{
	return _gameState;
}

int Morpion::getX() const
{
	return _x;
}

int Morpion::getY() const
{
	return _y;
}

void Morpion::setTurn(size_t turn)
{
	_turn = turn;
}

void Morpion::setGameState(std::string *gameState)
{
	_gameState = gameState;
}

void Morpion::setX(int x)
{
	_x = x;
}

void Morpion::setY(int y)
{
	_y = y;
}