#include "../connectfour.hpp"

ConnectFour::ConnectFour(std::string type, std::vector<std::string> players) : Game()
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "Connectfour game created" << std::endl;
	_turn = 0;
	_gameState = new std::string[6] {"       ", "       ", "       ", "       ", "       ", "       "};
}

ConnectFour::~ConnectFour()
{
	delete[] _gameState;
}

void ConnectFour::createRoom()
{
	std::cout << "create room" << std::endl;
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
			int x = startX, y = startY;

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

bool ConnectFour::winCondition()
{
	return checkDirection(0, 1) || checkDirection(1, 0) || checkDirection(1, 1) || checkDirection(1, -1);
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

bool ConnectFour::isBufferFull()
{
	if (winCondition() || _turn >= 43)
	{
		if (_turn >= 43)
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
			_buffer += "\n" "\x03" "4" "Player 1 turn\nInput a column number (1-7):";
		else
			_buffer += "\n" "\x03" "2" "Player 2 turn\nInput a column number (1-7):";
		return true;
	}
	if (!checkInput())
		return true;
	return false;
}


bool ConnectFour::checkStart()
{
	if (_turn == 0 && _input == "!start")
	{
		_buffer = "--------------------\n|   CONNECT FOUR   |\n--------------------\n";
		displayGame();
		_buffer += "\n" "\x03" "4" "Player 1 turn\nInput a column number (1-7):";
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

void ConnectFour::gameLoop()
{
	if (checkStart())
		return;
	if (isBufferFull())
		return;
	if (!checkInput())
		return;
	for (int i = 5; i >= 0; i--)
	{
		if ((i <= 5 && _gameState[i][_input[0] - '0' - 1] == ' '))
		{
			if (_turn % 2)
				_gameState[i][_input[0] - '0' - 1] = 'x';
			else
				_gameState[i][_input[0] - '0' - 1] = 'o';
			break;
		}
		else if (_gameState[0][_input[0] - '0' - 1] != ' ')
		{
			_buffer += "Column is full!\nInput a column number (1-7):";
			return;
		}
	}
	_turn++;
	displayGame();
	_input.clear();
	if (isBufferFull())
		return;
}
