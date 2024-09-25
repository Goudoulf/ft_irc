#include "../header/CommandBuilder.hpp"
#include <string>
#include <utility>

CommandBuilder::Builder& CommandBuilder::Builder::name(std::string name)
{
	this->_name = name;
	return *this;
}

CommandBuilder::Builder& CommandBuilder::Builder::param(std::string param)
{
	this->_params.push_back(param);
	return *this;
}

CommandBuilder::Builder& CommandBuilder::Builder::command(Command *command)
{
	this->_command = command;
	return *this;
}

const CommandBuilder *CommandBuilder::Builder::build() const
{
	return new CommandBuilder(_name, _params, _command);
}

const std::string CommandBuilder::getName()const
{
	return _name;
}

void    CommandBuilder::fill_param(int fd, std::vector<std::string>& param, IRCServer& server)
{
	std::vector<std::pair<std::string, std::string>>::iterator it = _params.begin();
	std::vector<std::string>::iterator it2 = param.begin();
	while ((it != _params.end()) && (it2 != param.end()))
	{
		it->second = *it2;
		it++;
		it2++;
	}
	_command->execute(fd, _params , server);
}

CommandBuilder::~CommandBuilder() {}
