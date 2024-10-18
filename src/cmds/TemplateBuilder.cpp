#include "TemplateBuilder.hpp"
#include "ParamTemplate.hpp"
#include "cmds.h"
#include "reply.h"
#include <string>
#include <utility>
#include "debug.h"
#include <sstream>

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

bool    TemplateBuilder::check_level(Client *client)const
{
	if (this->_name != "USER" && this->_name != "PASS" && client->GetLevel() >= _levelNeeded)
		return true;
	if (client->GetLevel() == _levelNeeded)
		return true;
	return false;
}

void    TemplateBuilder::fill_param(Client *client, std::vector<std::string>& param)const
{
	log(INFO, "Filling param for " + this->_name);
	if (!check_level(client))
	{
		rpl_send(client->GetSocket(), ERR_NOTREGISTERED());
		return ;
	}
	std::map<std::string, std::string> final;
	std::vector<std::pair<std::string, const ParamTemplate*>>::const_iterator it = _params.begin();
	std::vector<std::string>::iterator it2 = param.begin();
	while ((it != _params.end()))
	{
		log(INFO, "Param = " + it->first);
		if (it->second->_isOptional == false && it2 == param.end())
		{
			rpl_send(client->GetSocket(), ERR_NEEDMOREPARAMS(this->getName()));
			return ;
		}
		if (it->second->_isOptional == true && it2 == param.end())
		{
			it++;
			continue ;
		}
		if (it->second && !it->second->checkParam(client, (*it2)))
			return ;
		final.insert(std::pair<std::string, std::string>(it->first, *it2));
		it++;
		if (it2 != param.end())
			it2++;
	}
	_command->execute(client, final);
}

TemplateBuilder::~TemplateBuilder() {}
