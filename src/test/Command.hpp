#include <string>
#include <vector>


class Command {
public:
	virtual ~Command() {};
	virtual void execute(int client_fd, const std::vector<std::string>& params)= 0;
};
