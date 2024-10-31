#include "ModeCommand.hpp"
#include "IRCServer.hpp"
#include "Channel.hpp"

#include "debug.h"
#include "reply.h"
#include "cmds.h"

#include <sstream>
#include <string>

void	modeInvite(bool sign, std::vector<std::string> params, Client& client, Channel& channel)
{
	channel.setInviteOnly(sign);
	(void)params;
	(void)client;
}

void	modeTopic(bool sign, std::vector<std::string> params, Client& client, Channel& channel)
{
	channel.setIsTopicForOp(sign);
	(void)params;
	(void)client;
}

void	modeKey(bool sign, std::vector<std::string> params, Client& client, Channel& channel)
{
	if (!sign)
	{
		channel.setPassword("");
		return ;
	}
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); it++)
	{
		if ((*it).find(' ') <= (*it).length())
		{
			rplSend(client.getSocket(), ERR_INVALIDKEY(client.getNickname(), channel.getChannelName()));
			return ;
		}
		channel.setPassword(*it);
	}
}

void	modeOpe(bool sign, std::vector<std::string> params, Client& client, Channel& channel)
{
	IRCServer *server = IRCServer::getInstance();
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end() && params.size() < 5; it++)
	{
		if (!channel.inChannel(*it))
		{
			rplSend(client.getSocket(), ERR_USERNOTINCHANNEL(*it, channel.getChannelName()));
			return ;
		}
		channel.setOp(server->findClient(*it), sign);
	}
}

void	modeLimit(bool sign, std::vector<std::string> params, Client& client, Channel& channel)
{
	std::stringstream stream;
	channel.setIsLimited(sign);
	if (!sign)
	{
		channel.setLimitSize(MAX_CLIENTS);
		return ;
	}
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end() && params.size() < 5; it++)
	{
		unsigned int newLimit;
		stream << (*it);
		stream >> newLimit;
		if (stream.fail() || newLimit == 0)
		{
			rplSend(client.getSocket(), ERR_INVALIDMODEPARAM(client.getNickname(), channel.getChannelName(), "l", *it, "invalid limit"));
			return ;
		}
		channel.setLimitSize(newLimit);
	}
}

void	mapModeInit(std::map<char, void (*)(bool, std::vector<std::string>, Client&, Channel&)> &mapFunc)
{
	mapFunc.insert(std::make_pair('i', &modeInvite));
	mapFunc.insert(std::make_pair('t', &modeTopic));
	mapFunc.insert(std::make_pair('k', &modeKey));
	mapFunc.insert(std::make_pair('o', &modeOpe));
	mapFunc.insert(std::make_pair('l', &modeLimit));
}

void ModeCommand::execute(Client *client, const std::map<std::string, std::vector<std::string> >& params)
{
	log(CMD, client->getNickname() + ":_____mode_____");
	IRCServer *server = IRCServer::getInstance();
	Channel *channel = server->findChannel(params.find("channel")->second[0]);
	std::vector<std::string> iter = params.find("modes")->second;
	std::map<char, void(*)(bool, std::vector<std::string>, Client&, Channel&)> mapFunction;
	mapModeInit(mapFunction);
	for(std::vector<std::string>::iterator it = iter.begin(); it != iter.end(); it++)
	{
		std::vector<std::string> toProcess = split((*it), ' ');
		std::string modes = toProcess.front();
		bool sign = modes.at(0) == '+';
		for (unsigned int i = 1; i < modes.length(); i++)
			(mapFunction.find(modes.at(i)))->second(sign, toProcess, *client, *channel);
	}
}
