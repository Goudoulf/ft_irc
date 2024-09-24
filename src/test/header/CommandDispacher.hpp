#pragma once

#include "CommandBuilder.hpp"
#include <string>
#include <vector>
class CommandDispacher {
	public:
		
		void dispachCommand(int client_fd, const std::string& command, const std::vector<std::string>& params)
		{
			Command *cmd = CommandBuilder()
				.command("hello")
				.param("mow")
				.build();
			Command *cmd = builder.build(command);
			if (cmd)
			{
				cmd->execute(client_fd, params);
				delete cmd;
			}
			Command *join = CommandBuilder()
				.command("join");
				.param("channel");
				.param("key");
				.optionnalparam("vector channel");
				.optionnalparam("vector key");
				.checkchannel();
				.build();
		}
};

