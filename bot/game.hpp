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
		bool						_start;
		bool						_finished;
		std::string					_chanName;
		std::string					_input;
		std::string					_buffer;
		std::string					*_gameState;
		std::string					_currentPlayer;
		std::vector<std::string>	_players;

	public:
		Game();
		virtual ~Game() {};

		std::string 				generateChanId();

		virtual bool				checkInput() = 0;
		virtual bool				isBufferFull() = 0;
		virtual bool				checkStart() = 0;
		virtual bool				winCondition() = 0;
		virtual void				displayGame() = 0;
		virtual void				gameLoop() = 0;

		bool						isFinished();
		bool						isStarted();

		std::string					getInput() const;
		std::string					getBuffer() const;
		std::string					getChanName() const;
		std::vector<std::string>	getPlayers() const;

		void						cleanBuffer();
		void						setInput(std::string input);
		void						setCurrentPlayer(std::string client);
		void						setPlayers(std::vector<std::string> players);
};

#endif