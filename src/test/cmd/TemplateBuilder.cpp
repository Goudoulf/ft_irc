#include "../header/TemplateBuilder.hpp"
#include <string>
#include <utility>

TemplateBuilder::TemplateBuilder(const std::string& name, const std::vector<std::string>& params, Command *command)
{
	_name = name;
	_command = command;
	for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); it++)
	{
		_params.insert(std::pair<std::string, std::string>(*it, ""));
	}
}
TemplateBuilder::Builder::Builder()
{}

TemplateBuilder::Builder& TemplateBuilder::Builder::name(std::string name)
{
	this->_name = name;
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::param(std::string param)
{
	this->_params.push_back(param);
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::trailing(std::string param)
{
	this->_params.push_back(param);
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::command(Command *command)
{
	this->_command = command;
	return *this;
}

const TemplateBuilder *TemplateBuilder::Builder::build() const
{
	return new TemplateBuilder(_name, _params, _command);
}

const std::string TemplateBuilder::getName()const
{
	return _name;
}

void    TemplateBuilder::fill_param(int fd, std::vector<std::string>& param, IRCServer& server)const
{
	std::map<std::string, std::string> final;
	std::map<std::string, std::string>::const_iterator it = _params.begin();
	std::vector<std::string>::iterator it2 = param.begin();
	while ((it != _params.end()) && (it2 != param.end()))
	{
		final.insert(std::pair<std::string, std::string>(it->first, *it2));
		it++;
		it2++;
	}
	_command->execute(fd, final , server);
}

TemplateBuilder::~TemplateBuilder() {}
