#include "../bot.hpp"

int main (int argc, char **argv)
{
	struct sockaddr_in	address;
	int test = 1;
	int	myFd;
	u_int16_t _port;
	char *end;
	
	(void)argc;
	Bot *bot = new Bot(argv[1]);
	bot->run();

	return (0);
}