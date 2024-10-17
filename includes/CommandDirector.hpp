#pragma once
#include "TemplateBuilder.hpp"
#include <map>

class TemplateBuilder;

class CommandDirector {

	public:
	
		void	addCommand(const TemplateBuilder *command);
		void	parseCommand(int fd, std::string buffer);
		~CommandDirector();

	private:
		
		std::map<std::string, const TemplateBuilder *> _commandList;
};
