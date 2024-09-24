#pragma once

#include "CommandBuilder.hpp"
#include <string>
#include <vector>
class CommandDispacher {
	public:
		
		void dispachCommand(int client_fd, const std::string& command, const std::vector<std::string>& params)
		{
			CommandBuilder builder;
			Command *cmd = builder.build(command);
			if (cmd)
			{
				cmd->execute(client_fd, params);
				delete cmd;
			}
		}
};

