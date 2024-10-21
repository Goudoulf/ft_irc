#pragma once
#include "TemplateBuilder.hpp"
#include <map>

class TemplateBuilder;

class CommandDirector {

	public:
	
		void	addCommand(TemplateBuilder *command);
		void	parseCommand(Client* client, std::string buffer);
		~CommandDirector();

	private:
		
		std::map<std::string, TemplateBuilder *> _commandList;
};
