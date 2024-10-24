#pragma once
#include "TemplateBuilder.hpp"
#include "Client.hpp"
#include <map>

class	Client;
class TemplateBuilder;

class CommandDirector {

	public:
	
		void	addCommand(TemplateBuilder *command);
		void	parseCommand(Client* client, std::string buffer);
		~CommandDirector();

	private:
		
		std::map<std::string, TemplateBuilder *> _commandList;
};
