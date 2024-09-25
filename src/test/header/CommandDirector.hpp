#pragma once
#include "CommandBuilder.hpp"
#include <map>

class CommandDirector {

	public:
	
		void	addCommand(const CommandBuilder *command);
		void	parseCommand(int fd, std::string buffer, IRCServer& server);
		~CommandDirector();

	private:
		
		std::map<std::string, const CommandBuilder *> _commandList;
};
