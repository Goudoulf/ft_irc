#include "../hangman.hpp"

HangMan::HangMan(std::string type, std::vector<std::string> players) : Game()
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "HangMan game created" << std::endl;
	_attempt = 0;
	_gameState = new std::string[8] {
										"_______\n |    |\n |    o\n |   /|\\\n |   / \\\n_|_______",
										"_______\n |    |\n |    o\n |   /|\\\n |   / \n_|_______",
										"_______\n |    |\n |    o\n |   /|\\\n |\n_|_______",
										"_______\n |    |\n |    o\n |   /|\n |\n_|_______",
										"_______\n |    |\n |    o\n |    |\n |\n_|_______",
										"_______\n |    |\n |    o\n |\n |\n_|_______",
										"_______\n |    |\n |\n |\n |\n_|_______",
										""
										};
	setNewWordToGuess();
}

HangMan::~HangMan()
{
	delete[] _gameState;
}

void HangMan::createRoom()
{
	std::cout << "create room" << std::endl;
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

bool HangMan::winCondition()
{
	if (_input == _wordToGuess || _lettersGuessed == _wordToGuess)
		return true;
	return false;
}

bool HangMan::isBufferFull()
{
	if (_attempt >= 7 || winCondition())
	{
		if (_attempt >= 7)
			_buffer += "Oh no! You didn't guess the word " + _wordToGuess;
		else
			_buffer += "Well play! You guess the word "  + _wordToGuess;
		_finished = true;
		return true;
	}
	if (_input.empty())
	{
		_buffer += "\x02" "\x03" "2" + (*_itPlayer) + "\nInput a letter or a word:";
		return true;
	}
	if (!checkInput())
		return true;
	return false;
}

bool HangMan::checkStart()
{
	if (!_start && _input == "!start")
	{
		_buffer = "-------------------\n|     HANGMAN     |\n-------------------\n";
		displayGame();
		_itPlayer = _players.begin();
		_buffer += "\x02" "\x03" "2" + (*_itPlayer) + "\nInput a letter or a word: ";
		_start = true;
		return true;
	}
	else if (!_start)
	{
		_buffer.clear();
		return true;
	}
	return false;
}

void HangMan::gameLoop()
{
	if (checkStart())
		return;
	if (_currentPlayer != (*_itPlayer))
		return;
	if (isBufferFull())
		return;
	if (!checkInput())
		return;

	char c = _input[0];
	if (_input.length() == 1)
	{
		if (_lettersTried.find(c, 0) != std::string::npos)
		{
			_buffer += "This letter has been already put\nInput a letter or a word: ";
			return;
		}
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
		if (isBufferFull())
			return;
	}
	else if (_input != _wordToGuess)
	{
		for (std::vector<std::string>::iterator it = _wordsTried.begin(); it != _wordsTried.end(); it++)
		{
			if (*it == _input)
			{
				_buffer += "This word has been already put\nInput a letter or a word: ";
				return;
			}
		}
		_wordsTried.push_back(_input);
		_attempt++;
	}
	else if (isBufferFull())
		return;
	displayGame();
	_input.clear();
	if (++_itPlayer == _players.end())
		_itPlayer = _players.begin();
	if (isBufferFull())
		return;
}

void HangMan::setNewWordToGuess()
{
	char buff[50];
	std::ifstream dictionary;
	dictionary.open("easy_dictionary");
	srand(time(0));
	size_t line = (rand() % 853);
	for (size_t i = 0; i < line - 1; i++)
		dictionary.ignore(50, '\n');
	dictionary.getline(buff, 50);
	dictionary.close();
	_wordToGuess = buff;
	_lettersGuessed = std::string(_wordToGuess.length(), '-');
}
