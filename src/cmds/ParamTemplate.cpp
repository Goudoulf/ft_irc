#include "ParamTemplate.hpp"
#include "reply.h"

ParamTemplate::ParamTemplate(std::vector<bool (*)(const std::string, int fd , IRCServer& server)> checker, bool optional) : _paramCheckers(checker)
{
	_isOptional = optional;
}

ParamTemplate::Builder::Builder() : _isOptional(false)
{}

ParamTemplate::Builder& ParamTemplate::Builder::addChecker(bool (*ptr)(const std::string, int fd , IRCServer& server))
{
	this->_paramChecker.push_back(ptr);
	return *this;
}

ParamTemplate::Builder& ParamTemplate::Builder::isOptional()
{
	_isOptional = true;
	return *this;
}

const ParamTemplate *ParamTemplate::Builder::build() const
{
	return new ParamTemplate(_paramChecker, _isOptional);
}

bool    ParamTemplate::checkParam(int fd, const std::string& param, IRCServer& server)const
{
	(void)param;
	(void)fd;
	(void)server;
	for (std::vector<bool (*)(const std::string, int, IRCServer&)>::const_iterator it = _paramCheckers.begin(); it != _paramCheckers.end(); it++)
	{
		if ((*it)(param, fd, server) == false)
			return false;

	}
	return true;
}

ParamTemplate::~ParamTemplate() {}
