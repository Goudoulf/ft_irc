#include "ParamTemplate.hpp"

ParamTemplate::ParamTemplate(std::vector<bool (*)(const std::string, Client*)> checker, bool optional) : _paramCheckers(checker)
{
	_isOptional = optional;
}

ParamTemplate::Builder::Builder() : _isOptional(false)
{}

ParamTemplate::Builder& ParamTemplate::Builder::addChecker(bool (*ptr)(const std::string, Client *))
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

bool    ParamTemplate::checkParam(Client *client, std::vector<std::string> param) const
{
	for (std::vector<bool (*)(const std::string, Client*)>::const_iterator it = _paramCheckers.begin(); it != _paramCheckers.end(); it++)
	{
		for (std::vector<std::string>::iterator it2 = param.begin(); it2 != param.end(); it2++)
		{
			if ((*it)(*it2, client) == false)
				return false;
		}

	}
	return true;
}

ParamTemplate::~ParamTemplate() {}
