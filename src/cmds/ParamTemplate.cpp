#include "ParamTemplate.hpp"

ParamTemplate::ParamTemplate(std::vector<bool (*)(const std::string, int fd , IRCServer& server)> checker) : _paramCheckers(checker)
{}

ParamTemplate::Builder::Builder()
{}

ParamTemplate::Builder& ParamTemplate::Builder::addChecker(bool (*ptr)(const std::string, int fd , IRCServer& server))
{
	this->_paramChecker.push_back(ptr);
	return *this;
}

const ParamTemplate *ParamTemplate::Builder::build() const
{
	return new ParamTemplate(_paramChecker);
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
