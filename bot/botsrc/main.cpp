#include "../bot.hpp"

int main (int argc, char **argv)
{
	(void)argc;
	Bot *bot = new Bot(argv[1]);
	bot->run();
	delete (bot);
	return (0);
}