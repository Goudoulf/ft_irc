#include "../bot.hpp"

int main (int argc, char **argv)
{
	struct sockaddr_in	address;
	int test = 1;
	int	myFd;
	u_int16_t _port;
	char *end;
	char buffer[1024];
	std::string port(argv[1]);
	memset(&address, 0, sizeof(address));
	(void)argc;
    if ((myFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
        std::cout << "ERROR socket" << std::endl;
	}
    if (setsockopt(myFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &test, sizeof(test)))
	{
        std::cout << "ERROR socketopt" << std::endl;
	}
	_port = static_cast<unsigned short>(std::strtod(port.c_str(), &end)); 
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
	if (connect(myFd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << "ERROR CONNECT" << std::endl;
		return (1);
	}
	sleep(3);
	send(myFd, "PASS tutu\r\n", 11, 0);
	sleep(2);
	send(myFd, "NICK bot\r\nUSER bot 0 * :realname\r\n", 34, 0);
	sleep(2);
	send(myFd, "JOIN #botchan\r\n", 15, 0);
	while (1)
	{
		bzero(buffer, 1024);
		recv(myFd, buffer, 1024, 0);
		std::cout << buffer << std::endl;
	}
	return (0);
}