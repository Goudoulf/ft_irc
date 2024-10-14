#include "../connectfour.hpp"

ConnectFour::ConnectFour(std::string type, std::vector<std::string> players)
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "Connectfour game created" << std::endl;
	_turn = 1;
	_gameState = new std::string[6] {"       ", "       ", "       ", "       ", "       ", "       "};
	_buffer = "--------------------\n|   CONNECT FOUR   |\n--------------------\n";
	displayGame();
}

ConnectFour::~ConnectFour()
{
	delete[] _gameState;
}

void ConnectFour::createRoom()
{
	std::cout << "create room" << std::endl;
}

bool ConnectFour::winCondition()
{
	int connected;
	char past;

	//Vertical Check
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (j != 0 && _gameState[j][i] == past && past != ' ')
			{
				connected++;
				if (connected == 4)
					return true;
			}
			else
				connected = 1;
			past = _gameState[j][i];
		}
	}

	//Horizontal Check
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (j != 0 && _gameState[i][j] == past && past != ' ')
			{
				connected++;
				if (connected == 4)
					return true;
			}
			else
				connected = 1;
			past = _gameState[i][j];
		}
	}

	//Diagonal Check
	for (int i = 0; i < 7; i++)
	{
		int k;
		if (i <= 3)
		{
			past = ' ';
			k = i;
			for (int j = 0; j < 6 && k < 7; j++)
			{
				if (j != 0 && _gameState[j][k] == past && past != ' ')
				{
					connected++;
					if (connected == 4)
						return true;
				}
				else
					connected = 1;
				past = _gameState[j][k];
				k++;
			}
		}
		if (i >= 3)
		{
			past = ' ';
			k = i;
			for (int j = 0; j < 6 && k < 7; j++)
			{
				if (j != 0 && _gameState[j][k] == past && past != ' ')
				{
					connected++;
					if (connected == 4)
						return true;
				}
				else
					connected = 1;
				past = _gameState[j][k];
				k--;
			}
		}
	}

	for (int i = 1; i < 3; i++)
	{
		int k = i;
		for (int j = 0; j < 7 && k < 6; j++)
		{
			if (j != 0 && _gameState[k][j] == past && past != ' ')
			{
				connected++;
				if (connected == 4)
					return true;
			}
			else
				connected = 1;
			past = _gameState[k][j];
			k++;
		}
	}

	for (int i = 1; i < 3; i++)
	{
		int k = i;
		for (int j = 6; j >= 0 && k < 6; j--)
		{
			if (j != 0 && _gameState[k][j] == past && past != ' ')
			{
				connected++;
				if (connected == 4)
					return true;
			}
			else
				connected = 1;
			past = _gameState[k][j];
			k++;
		}
	}
	return false;
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
			_buffer += "\n\e[1;31mPlayer 1 turn\e[0m\nInput a column number (1-7):";
		else
			_buffer += "\n\e[1;32mPlayer 2 turn\e[0m\nInput a column number (1-7):";
		return true;
	}
	if (!checkInput())
		return true;
	return false;
}

void ConnectFour::gameLoop()
{
	if (isBufferFull())
		return;
	if (!checkInput())
		return;
	for (int i = 0; i < 6; i++)
	{
		if ((i < 5 && _gameState[i + 1][_input[0] - '0' - 1] != ' ') || (i == 5 && _gameState[i][_input[0] - '0' - 1] == ' '))
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
