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

	public:
		Game();
		// Game(std::vector<std::string> params);
		// virtual ~Game();
		// Game(const Game& myGame);
		// const Game& operator=(const Game& myGame);
		std::string getChanName();
		std::string generateChanId();
		std::vector<std::string> getPlayers();
		void setPlayers(std::vector<std::string> players);
		virtual void createRoom() = 0;
		// virtual void newTurn() = 0;
		
};

#endif