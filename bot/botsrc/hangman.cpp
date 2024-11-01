#include "../hangman.hpp"

HangMan::HangMan(std::string type, std::vector<std::string> players) : Game()
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "HangMan game created" << std::endl;
	_attempt = 0;
	_gameState = new std::string[8];
	_gameState[0] = "_______\n |    |\n |    o\n |   /|\\\n |   / \\\n_|_______";
	_gameState[1] = "_______\n |    |\n |    o\n |   /|\\\n |   / \n_|_______";
	_gameState[2] = "_______\n |    |\n |    o\n |   /|\\\n |\n_|_______";
	_gameState[3] = "_______\n |    |\n |    o\n |   /|\n |\n_|_______";
	_gameState[4] = "_______\n |    |\n |    o\n |    |\n |\n_|_______";
	_gameState[5] = "_______\n |    |\n |    o\n |\n |\n_|_______";
	_gameState[6] = "_______\n |    |\n |\n |\n |\n_|_______";
	_gameState[7] = "";
	setNewWordToGuess();
}

HangMan::~HangMan()
{
	delete[] _gameState;
}

bool HangMan::isGameReady()
{
	if (checkStart() || isBufferFull())
		return false;
	return true;
}

bool HangMan::isPlayerTurn()
{
	if (_currentPlayer != (*_itPlayer))
		return false;
	return true;
}

bool HangMan::isInputValid()
{
	if (!checkInput())
		return false;

	char c = _input[0];
	if (_input.length() == 1 && _lettersTried.find(c, 0) != std::string::npos)
	{
		_buffer += "This letter has been already put!\nInput a letter or a word: ";
		return false;
	}
	else if (_input != _wordToGuess)
	{
		if (std::find(_wordsTried.begin(), _wordsTried.end(), _input) != _wordsTried.end())
		{
			_buffer += "This word has been already put!\nInput a letter or a word: ";
			return false;
		}
	}
	return true;
}

void HangMan::updateGameState()
{
	char c = _input[0];
	if (_input.length() == 1)
	{
		_lettersTried += c;
		size_t pos = 0;
		if (_wordToGuess.find(c, pos) == std::string::npos)
			_attempt++;
		else
			while ((pos = _wordToGuess.find(c, pos)) != std::string::npos)
			{
				_lettersGuessed[pos] = _wordToGuess[pos];
				pos++;
			}
	}
	else
	{
		_wordsTried.push_back(_input);
		_attempt++;
	}
	if (++_itPlayer == _players.end())
		_itPlayer = _players.begin();
	_input.clear();
}

void HangMan::displayGame()
{
	_buffer += _gameState[7 - _attempt] + "\n";
	_buffer += "\x03" "3" + _lettersGuessed + "\n";
	_buffer += "\x03" "4" + _lettersTried;
	if (!_wordsTried.empty())
		_buffer += " | ";
	for (std::vector<std::string>::iterator it = _wordsTried.begin(); it != _wordsTried.end(); it++)
	{
		if (it != _wordsTried.begin())
			_buffer += ", ";
		_buffer += (*it);
	}
	_buffer += '\n';
	if (checkGameOver())
		return;
	_buffer += "\x02" "\x03" "2" + (*_itPlayer) + "'s turn\nInput a letter or a word: ";
}

bool HangMan::isBufferFull()
{
	if (checkGameOver())
		return true;

	if (_input.empty())
	{
		_buffer += "\x02" "\x03" "2" + (*_itPlayer) + "'s turn\nInput a letter or a word:";
		return true;
	}
	return !checkInput();
}

bool HangMan::checkStart()
{
	if (_start)
		return false;
	if (_input == "!start")
		return handleStartCommand();
	
	_buffer.clear();
	return true;
}

bool HangMan::checkInput()
{
	for (size_t i = 0; i < _input.length(); i++)
	{
		if (!isalpha(_input[i]))
		{
			_buffer += "Your input is not a letter or a word!\nInput a letter: ";
			return false;
		}
		else if (_input[i] >= 'A' && _input[i] <= 'Z')
			_input[i] += 'a' - 'A';
	}
	return true;
}

bool HangMan::handleStartCommand()
{
	_itPlayer = _players.begin();
	prepareGameStartMessage();
	_start = true;
	return true;
}

bool HangMan::checkGameOver()
{
	if (_attempt >= 7 || winCondition())
	{
		if (_attempt >= 7)
			_buffer += "Oh no! You didn't guess the word " "\x03" "3" + _wordToGuess;
		else
			_buffer += "Well play " + _currentPlayer + "! You guess the word " "\x03" "3" + _wordToGuess;
		_finished = true;
		return true;
	}
	return false;
}

void HangMan::prepareGameStartMessage()
{
	_buffer = "-------------------\n|     HANGMAN     |\n-------------------\n";
	displayGame();
}

void HangMan::initializePlayers()
{
}

bool HangMan::winCondition()
{
	if (_input == _wordToGuess || _lettersGuessed == _wordToGuess)
		return true;
	return false;
}

void HangMan::setNewWordToGuess()
{
	char buff[50];
	std::ifstream dictionary;
	dictionary.open("easy_dictionary");

	if (!dictionary) {
		_buffer += "Error to start the game! Please create a new game.";
		_error = true;
		_finished = true;
		return;
	}

	srand(time(0));
	size_t line = (rand() % 852);
	for (size_t i = 0; i < line - 1; i++)
		dictionary.ignore(50, '\n');
	dictionary.getline(buff, 50);
	dictionary.close();
	_wordToGuess = buff;
	_lettersGuessed = std::string(_wordToGuess.length(), '-');
}
