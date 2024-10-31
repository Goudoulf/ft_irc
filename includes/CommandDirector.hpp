#ifndef COMMANDDIRECTOR_HPP
#define COMMANDDIRECTOR_HPP

#include "TemplateBuilder.hpp"
#include "Client.hpp"
#include <map>

class	TemplateBuilder;
class	Client;

class CommandDirector {

	public:
	
		void	addCommand(TemplateBuilder *command);
		void	parseCommand(Client* client, std::string buffer);
		~CommandDirector();

	private:
		
		std::map<std::string, TemplateBuilder *> _commandList;
};

#endif
