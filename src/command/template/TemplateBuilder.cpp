#include "TemplateBuilder.hpp"
#include "CommandParser.hpp"
#include "DefaultCommandParser.hpp"
#include "ParamTemplate.hpp"

#include "reply.h"
#include "debug.h"

#include <string>
#include <map>

TemplateBuilder::TemplateBuilder(const std::string& name, CmdLevel level, const std::vector<std::pair<std::string, const ParamTemplate*> >& params , Command *command, CommandParser *parser)
{
	_name = name;
	_command = command;
	_levelNeeded = level;
	_parser = parser;
	for (std::vector<std::pair<std::string, const ParamTemplate*> >::const_iterator it = params.begin(); it != params.end(); it++)
		this->_params.push_back(std::make_pair(it->first, it->second));
}

TemplateBuilder::Builder::Builder(): _parser(new DefaultCommandParser()) {}

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
	delete this->_parser;
	this->_parser = parser;
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

bool    TemplateBuilder::checkLevel(Client *client)const
{

	if ((this->getName() == "PASS" || this->getName() == "USER") && client->getLevel() > _levelNeeded)
	{
		rplSend(client->getSocket(), ERR_ALREADYREGISTRED(this->getName()));
		return false;
	}
	if (client->getLevel() < _levelNeeded)
	{
		rplSend(client->getSocket(), ERR_NOTREGISTERED(this->getName()));
		return false;
	}
	if (client->getLevel() >= _levelNeeded)
		return true;
	return false;
}

bool	TemplateBuilder::fillParam(Client *client, std::vector<std::vector<std::string> > param)
{
	log(INFO, "Filling param for " + this->_name);
	std::vector<std::pair<std::string, const ParamTemplate*> >::iterator it = _params.begin();
	std::vector<std::vector<std::string> >::iterator it2 = param.begin();
	while ((it != _params.end()))
	{
		log(INFO, "Param = " + it->first);
		if (it->second->_isOptional == false && it2 == param.end())
		{
			client->replyServer(ERR_NEEDMOREPARAMS(this->getName()));
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
	if (it2 != param.end())
	{
		client->replyServer(ERR_NEEDMOREPARAMS(this->getName()));
		return false;
	}
	return true;
}

void    TemplateBuilder::executeCommand(Client *client, const std::string &input)
{
	log(DEBUG, "input =" + input + "|");
	std::vector<std::vector<std::string> > params;
	if (!_parser->parse(input, params))
		return;
	if (!checkLevel(client))
		return ;
	if (!fillParam(client, params))
		return;
	_command->execute(client, _parsedParams);
	_parsedParams.clear();
}

TemplateBuilder::~TemplateBuilder()
{
	if (_command)
		delete _command;
	if (_parser)
		delete _parser;
	std::vector<std::pair<std::string, const ParamTemplate*> >::iterator it;
	for (it = _params.begin(); it != _params.end(); it++)
	{
		if (it->second)
			delete it->second;
	}
	_params.clear();
}
