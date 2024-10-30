#include "../connectfour.hpp"

ConnectFour::ConnectFour(std::string type, std::vector<std::string> players) : Game()
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "Connectfour game created" << std::endl;
	_turn = 0;
	_gameState = new std::string[6];
	for (int i = 0; i < 6; i++)
		_gameState[i] = "       ";
}

ConnectFour::~ConnectFour()
{
	delete[] _gameState;
}

bool ConnectFour::isGameReady()
{
	if (checkStart() || isBufferFull())
		return false;
	return true;
}

bool ConnectFour::isPlayerTurn()
{
	if ((_turn % 2 && _currentPlayer != _player1) ||
		(!(_turn % 2) && _currentPlayer != _player2))
		return false;
	return true;
}

bool ConnectFour::isInputValid()
{
	if (!checkInput())
		return false;
	int x = _input[0] - '0' - 1;
	
	if (_gameState[0][x] != ' ')
	{
		_buffer += "Column is full!\nInput a column number (1-7):";
		return false;
	}
	return true;
}

void ConnectFour::updateGameState()
{
	for (int i = 5; i >= 0; i--)
	{
		if ((i <= 5 && _gameState[i][_input[0] - '0' - 1] == ' '))
		{
			_gameState[i][_input[0] - '0' - 1] = (_turn % 2) ? 'x' : 'o';
			break;
		}
	}
	_turn++;
	_input.clear();
}

void ConnectFour::displayGame()
{
	_buffer += " 1 2 3 4 5 6 7 \n _ _ _ _ _ _ _ \n";
	for (int i = 0; i < 6; i++)
	{
		_buffer += '|';
		for (int j = 0; j < 7; j++)
		{
			if (_gameState[i][j] == ' ')
				_buffer += '_';
			else
				_buffer += _gameState[i][j];
			_buffer += '|';
		}
		_buffer += '\n';
	}
}

bool ConnectFour::checkStart()
{
	if (_start)
		return false;
	if (_input == "!start")
		return handleStartCommand();
	
	_buffer.clear();
	return true;
}

bool ConnectFour::isBufferFull()
{
	if (checkGameOver())
		return true;

	if (_input.empty())
	{
		std::string player = (_turn % 2) ? _player2 : _player1;
		std::string colorCode = (_turn % 2) ? "\x03" "4" : "\x03" "2";
		_buffer += "\n" + colorCode + player + "'s turn\nInput a column number (1-7):";
		return true;
	}
	return !checkInput();
}

bool ConnectFour::checkInput()
{
	if (_input.length() != 1 || _input[0] < '1' || _input[0] > '7')
	{
		_buffer += "Input is not a column number (1-7)!\nInput a column number:";
		return false;
	}
	return true;
}

bool ConnectFour::handleStartCommand()
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

bool ConnectFour::checkGameOver()
{
	if (winCondition() || _turn >= 43)
	{
		if (_turn >= 43)
			_buffer += "Draw!";
		else
			_buffer += (_turn % 2 ? _player2 : _player1) + " wins!";
		_finished = true;
		return true;
	}
	return false;
}

void ConnectFour::prepareGameStartMessage()
{
	_buffer = "--------------------\n|   CONNECT FOUR   |\n--------------------\n";
	displayGame();
}

void ConnectFour::initializePlayers()
{
	_player1 = _players[0];
	_player2 = _players[1];
}

bool ConnectFour::winCondition()
{
	return checkDirection(0, 1) || checkDirection(1, 0) || checkDirection(1, 1) || checkDirection(1, -1);
}

bool ConnectFour::checkDirection(int deltaX, int deltaY)
{
	for (int startX = 0; startX < 7; startX++)
	{
		for (int startY = 0; startY < 6; startY++)
		{
			char past = _gameState[startY][startX];
			if (past == ' ')
				continue;
			int connected = 1;
			int x = startX;
			int y = startY;

			while (true)
			{
				x += deltaX;
				y += deltaY;
				if (x < 0 || x >= 7 || y < 0 || y >= 6)
					break;
				if (_gameState[y][x] == past)
				{
					connected++;
					if (connected == 4)
						return true;
				}
				else
					break;
			}
		}
	}
	return false;
}
