#include "header/CommandBuilder.hpp"
#include "header/CommandDirector.hpp"
#include "header/JoinCommand.hpp"
#include "header/PartCommand.hpp"

void cmd()
{
	CommandDirector director;

	director.addCommand(CommandBuilder::Builder()
					 .name("JOIN")
					 .param("channel")
					 .param("key")
					 .command(new JoinCommand())
					 .build()
					 );

	director.addCommand(CommandBuilder::Builder()
					 .name("PART")
					 .param("channel")
					 .command(new PartCommand())
					 .build()
					 );
}
