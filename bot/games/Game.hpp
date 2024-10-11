#ifndef GAME_HPP
#define GAME_HPP

#include <string>

class Game
{
	public:
		Game();
		virtual ~Game();

		virtual std::string getName() const = 0;
		std::string			getInput() const;
		std::string			getBuffer() const;

		void				setName(std::string name);
		void				setInput(std::string input);
		void				setBuffer(std::string buffer);

	protected:
		std::string _name;
		std::string	_input;
		std::string _buffer;
};

#endif