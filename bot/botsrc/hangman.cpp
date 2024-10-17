#include "../hangman.hpp"

HangMan::HangMan(std::string type, std::vector<std::string> players)
{	
	_players = players;
	_chanName = "#" + type + generateChanId();
	std::cout << "HangMan game created" << std::endl;
	_attempt = 0;
	_c = 0;
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
	_finished = false;
	_start = false;
}

HangMan::~HangMan()
{
	delete[] _gameState;
}

void HangMan::createRoom()
{
	std::cout << "create room" << std::endl;
}

bool HangMan::winCondition()
{
	return true;
}

void	HangMan::cleanBuffer()
{
	_buffer.clear();
}

void HangMan::displayGame()
{
	_buffer += _gameState[7 - _attempt] + "\n";
	_buffer += "\x03" "3" + _lettersGuessed + "\n";
	_buffer += "\x03" "4" + _lettersTried + " | ";
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
	return true;
}

bool HangMan::isBufferFull()
{
	if (_attempt >= 7 || _finished)
	{
		if (_attempt >= 7)
			_buffer += "Oh no! You didn't guess the word " + _wordToGuess;
		else
			_buffer += "Well play! You guess the word"  + _wordToGuess;
		_finished = true;
		return true;
	}
	if (_input.empty())
	{
		std::cout << "BUFFERFULL" << std::endl;
		_buffer += "Input a letter or a word:";
		return true;
	}
	if (!checkInput())
		return true;
	return false;
}

void HangMan::gameLoop()
{
	if (!_start && _input == "!start")
	{
		_buffer = "--------------------\n|   CONNECT FOUR   |\n--------------------\n";
		displayGame();
		_buffer += "\nInput a letter or a word: ";
		_start = true;
		return;
	}
	else if (!_start)
	{
		_buffer.clear();
		return;
	}
	if (isBufferFull())
		return;
	if (!checkInput())
		return;

	_c = _input[0];
	if (_input.length() == 1)
	{
		if (_lettersTried.find(_c, 0) != std::string::npos)
		{
			_buffer += "This letter has been already put\nInput a letter or a word: ";
			return;
		}
		_lettersTried += _c;
		size_t pos = 0;
		if (_wordToGuess.find(_c, pos) == std::string::npos)
			_attempt++;
		else
			while ((pos = _wordToGuess.find(_c, pos)) != std::string::npos)
			{
				_lettersGuessed[pos] = _wordToGuess[pos];
				pos++;
			}
		if (_lettersGuessed.find("-", 0) == std::string::npos)
		{
			_buffer += "\x03" "3" + _lettersGuessed;
			_finished = true;
			return;
		}
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
	else
	{
		_finished = true;
		if (isBufferFull())
			return;
	}
	displayGame();
	_input.clear();
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
