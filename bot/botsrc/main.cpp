#include "../bot.hpp"
#include "../debug.h"
#include "../color.h"

int main (int argc, char **argv)
{
	if (argc != 3)
	{
		log(ERROR, "Wrong number of arguments: ./bot <port> <password>");
		return (1);
	}
	Bot *bot = new Bot();
	if (bot->initialize(argv[1], argv[2]))
		bot->run();
	delete (bot);
	return (0);
}