#include "TemplateBuilder.hpp"
#include "CommandParser.hpp"
#include "DefaultCommandParser.hpp"
#include "ParamTemplate.hpp"
#include "cmds.h"
#include "reply.h"
#include <string>
#include <utility>
#include "debug.h"
#include <map>
#include <sstream>

TemplateBuilder::TemplateBuilder(const std::string& name, CmdLevel level, const std::vector<std::pair<std::string, const ParamTemplate*>>& params , Command *command, CommandParser *parser)
{
	_name = name;
	_command = command;
	_levelNeeded = level;
	_parser = parser;
	for (std::vector<std::pair<std::string, const ParamTemplate*>>::const_iterator it = params.begin(); it != params.end(); it++)
		this->_params.push_back(std::make_pair(it->first, it->second));
}

TemplateBuilder::Builder::Builder(): _parser(new DefaultCommandParser())
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

TemplateBuilder::Builder& TemplateBuilder::Builder::parser(CommandParser *parser)
{
	this->_parser = parser;
	// TODO: deep copy to clean delete
	return *this;
}

TemplateBuilder *TemplateBuilder::Builder::build()
{
	return new TemplateBuilder(_name, _level, _params, _command, _parser);
}

const std::string TemplateBuilder::getName()const
{
	return _name;
}

bool    TemplateBuilder::check_level(Client *client)const
{
	if (this->_name != "USER" && this->_name != "PASS" && client->getLevel() >= _levelNeeded)
		return true;
	if (client->getLevel() == _levelNeeded)
		return true;
	return false;
}

bool	TemplateBuilder::fill_param(Client *client, std::vector<std::vector<std::string>> param)
{
	log(INFO, "Filling param for " + this->_name);
	std::vector<std::pair<std::string, const ParamTemplate*>>::iterator it = _params.begin();
	std::vector<std::vector<std::string>>::iterator it2 = param.begin();
	while ((it != _params.end()))
	{
		log(INFO, "Param = " + it->first);
		if (it->second->_isOptional == false && it2 == param.end())
		{
			rpl_send(client->getSocket(), ERR_NEEDMOREPARAMS(this->getName()));
			return false;
		}
		if (it->second->_isOptional == true && it2 == param.end())
		{
			it++;
			continue ;
		}
		if (it->second && !it->second->checkParam(client, (*it2)))
			return false;
		this->_parsedParams.insert(std::make_pair(it->first, *it2));
		it++;
		if (it2 != param.end())
			it2++;
	}
	return true;
}

void    TemplateBuilder::executeCommand(Client *client, const std::string &input)
{
	(void)client;
	log(DEBUG, "input =" + input + "|");
	std::vector<std::vector<std::string>> params;
	if (!_parser->parse(input, params))
		return;
	if (!check_level(client))
	{
		rpl_send(client->getSocket(), ERR_NOTREGISTERED());
		return ;
	}
	if (!fill_param(client, params))
		return;
	_command->execute(client, _parsedParams);
	_parsedParams.clear();
}

TemplateBuilder::~TemplateBuilder() {}
