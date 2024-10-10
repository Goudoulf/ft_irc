#include "TemplateBuilder.hpp"
#include "ParamTemplate.hpp"
#include "cmds.h"
#include <string>
#include <utility>

TemplateBuilder::TemplateBuilder(const std::string& name, CmdLevel level, const std::vector<std::pair<std::string, const ParamTemplate*>>& params , Command *command)
{
	_name = name;
	_command = command;
	_levelNeeded = level;
	for (std::vector<std::pair<std::string, const ParamTemplate*>>::const_iterator it = params.begin(); it != params.end(); it++)
		this->_params.push_back(std::make_pair(it->first, it->second));
}

TemplateBuilder::Builder::Builder()
{}

TemplateBuilder::Builder& TemplateBuilder::Builder::name(std::string name)
{
	this->_name = name;
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::param(std::string type, const ParamTemplate *checker)
{
	this->_params.push_back(std::make_pair(type, checker));
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::trailing(std::string param, const ParamTemplate *checker)
{
	this->_params.push_back(std::make_pair(param, checker));
	return *this;
}
TemplateBuilder::Builder& TemplateBuilder::Builder::level(CmdLevel level)
{
	_level = level;	
	return *this;
}

TemplateBuilder::Builder& TemplateBuilder::Builder::command(Command *command)
{
	this->_command = command;
	return *this;
}

const TemplateBuilder *TemplateBuilder::Builder::build() const
{
	return new TemplateBuilder(_name, _level, _params, _command);
}

const std::string TemplateBuilder::getName()const
{
	return _name;
}

bool    TemplateBuilder::check_level(int fd, IRCServer& server)
{
        Client* client = (server.getClients()->find(fd))->second;
		if (client->GetLevel() >= _levelNeeded)
			return true;
		return false;
}

void    TemplateBuilder::fill_param(int fd, std::vector<std::string>& param, IRCServer& server)const
{
	std::map<std::string, std::string> final;
	std::vector<std::pair<std::string, const ParamTemplate*>>::const_iterator it = _params.begin();
	std::vector<std::string>::iterator it2 = param.begin();
	while ((it != _params.end()))
	{
		if (it->second->_isOptional == false && it2 == param.end())
		{
			rpl_send(fd, ERR_NEEDMOREPARAMS(this->getName()));
			return ;
		}
		if (it->second->_isOptional == true && it2 == param.end())
		{
			it++;
			continue ;
		}
		if (it->second && !it->second->checkParam(fd, (*it2), server))
			return ;
		final.insert(std::pair<std::string, std::string>(it->first, *it2));
		it++;
		if (it2 != param.end())
			it2++;
	}
	_command->execute(fd, final , server);
}

TemplateBuilder::~TemplateBuilder() {}
