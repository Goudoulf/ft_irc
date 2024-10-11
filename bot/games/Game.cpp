#include "Game.hpp"

Game::Game()
{}

Game::~Game()
{}

std::string Game::getName() const
{
	return _name;
}

std::string Game::getInput() const
{
	return _input;
}

void Game::setName(std::string name)
{
	_name = name;
}

void Game::setInput(std::string input)
{
	_input = input;
}

void Game::setBuffer(std::string buffer)
{
	_buffer = buffer;
}
