#ifndef GAME_HPP
# define GAME_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <iostream>
# include <string.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>

class Game
{
	protected:
		std::string _chanName;
		std::vector<std::string> _players;
		std::string	_input;
		std::string	_buffer;
		std::string	*_gameState;
		bool		_finished;

	public:
		Game();
		virtual ~Game();
		// Game(std::vector<std::string> params);
		// Game(const Game& myGame);
		// const Game& operator=(const Game& myGame);
		std::string getChanName();
		std::string generateChanId();
		std::vector<std::string> getPlayers();
		void setPlayers(std::vector<std::string> players);
		virtual void createRoom() = 0;

		//virtual void	resetGameState() = 0;
		virtual bool	winCondition() = 0;
		virtual void	gameLoop() = 0;
		bool			isFinished();

		std::string		getInput() const;
		std::string		getBuffer() const;

		void			cleanBuffer();
		void			setInput(std::string input);
		// virtual void newTurn() = 0;
		
};

#endif