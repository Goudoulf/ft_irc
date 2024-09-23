
class Command {
public:
	virtual ~Command() {};
	virtual void handle(int client_fd) const = 0;
};
