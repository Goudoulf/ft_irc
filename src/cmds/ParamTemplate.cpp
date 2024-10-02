#include "ParamTemplate.hpp"

ParamTemplate::ParamTemplate(std::vector<bool (*)(const std::vector<std::string>)> checker) : _paramCheckers(checker)
{}

ParamTemplate::Builder::Builder()
{}

ParamTemplate::Builder& ParamTemplate::Builder::addChecker(bool (*ptr)(const std::vector<std::string>))
{
	this->_paramChecker.push_back(ptr);
	return *this;
}

const ParamTemplate *ParamTemplate::Builder::build() const
{
	return new ParamTemplate(_paramChecker);
}

void    ParamTemplate::checkParam(int fd, std::vector<std::string>& param, IRCServer& server)
{
	(void)param;
	(void)fd;
	(void)server;
	for (std::vector<bool (*)(const std::vector<std::string>)>::const_iterator it = _paramCheckers.begin(); it != _paramCheckers.end(); it++)
	{
		if ((*it)(param) == false)
		{
			_isValid = false;
			break ;
		}
	}

}

ParamTemplate::~ParamTemplate() {}
