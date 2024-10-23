#pragma once
// #ifndef CHANNEL_HPP
// # define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"
#include <vector>
#include <ctime>
//& = local server, useless on this project. standard channel
//+ = channel modes not supported -> no one is OP. standard channel
//! = user creating the channel gets channel creator status -> operator. safe channel
//# = . standard channel

//standard channel = created when first user joins, destroyed when last user leave,
// anyone can reference the channel with it name, creating the channel gives OP,

//safe channel = the channel creator only choses a part of the channel name (channel "short name").
// the server then gives him a channel identifier (5 characters based on timeofday) the channel name becomes a combination
// of the short name + identifier can't create another channel with the same short name existing.
// ceases to exist when last user leaves (and no other member recently leaving because of network split -> useless).
enum channelMode {local, noMode, safe, standard, error}; //{&, +, !, #}

class Client;
//channel delay = not being able to recreate a channel remotely for a defined time.
class Channel
{
	public:
		Channel(const std::string &name, Client *creator, const std::string &key);
		~Channel();
		std::string getChannelName();
		std::string getUsers();
		std::string getPassword();
		channelMode getChannelMode();
		bool	InChannel(std::string client);
		bool	IsOp(std::string client);
		bool	keyIsValid(std::string &key);
		void	add_client(Client *client);
		void	addInvitation(Client *client);
		void	remove_client(Client *client);
		void	setTopic(std::string topic);
		std::string	getTopic();
		void	setInviteOnly(bool sign);
		bool	getInviteOnly(void);
		void	setIsTopicForOp(bool signe);
		bool	getIsTopicForOp(void);
		void	setIsLimited(bool sign);
		bool	getIsLimited(void);
		bool	getIsEmpty(void);
		void	setLimitSize(unsigned int limit);
		void	sendMessage(Client *sender, std::string message);
		void	sendReply(std::string message);
		unsigned int		getLimitSize(void);
		std::map<Client*, bool> getUsersMap(void);
		std::vector<Client *>	getInvitationList();
		
	private:
		std::string _name; //beginning with a &#+! length of 50 max char, case insensitive
							// no spaces or control G (ASCII 7), no ',' or ':', can't be reused
		//container of users, maybe a map <client, bool isOp> ?
		
		std::map<Client*, bool> _users;
		std::vector<Client *> _invited;
		// std::vector<Client> _users;
		// std::vector<Client> _operators;
		std::string			_topic;
		std::string			_password;
		bool				_isEmpty;
		bool				_InviteOnly;
		bool				_isTopicForOp;
		bool				_isLimited;

		
		unsigned int					_limitSize;
		channelMode _mode;//channel mode, depending on name prefix
		//stack of strings to make a message history if needed.

};

// #endif
