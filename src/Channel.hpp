#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"
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

//channel delay = not being able to recreate a channel remotely for a defined time.
class Channel
{
	public:
		Channel(const std::string &name, const Client &creator);
		~Channel();
		std::string getChannelName();
		channelMode getChannelMode();
		
		class InvalidName: public std::exception {
			public:
				virtual const char* what() const throw() {return "TODO";}
		};


	private:
		std::string _name; //beginning with a &#+! length of 50 max char, case insensitive
							// no spaces or control G (ASCII 7), no ',' or ':', can't be reused
		//container of users, maybe a map <client, bool isOp> ?
		std::vector<Client> _users;
		std::vector<Client> _operators;
		std::string	topic;
		channelMode _mode;//channel mode, depending on name prefix
		//stack of strings to make a message history if needed.

};

#endif