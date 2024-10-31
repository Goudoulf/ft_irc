#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

#include <string>
#include <map>
#include <vector>

class Client;

enum channelMode {local, noMode, safe, standard, error}; 

class Channel
{
	public:

		Channel(const std::string &name, Client *creator, const std::string &key);
		~Channel();

		void	sendMessage(Client *sender, std::string message);
		void	sendReply(std::string message);
		void	sendMessageWithSender(Client *sender, std::string message);
		void	addClient(Client *client);
		void	addInvitation(Client *client);
		void	removeClient(Client *client);
		bool	inChannel(std::string client);
		bool	isOp(std::string client);
		bool	keyIsValid(std::string &key);
		channelMode	selectMode(std::string name);

		void	setTopic(std::string topic);
		void	setInviteOnly(bool sign);
		void	setIsTopicForOp(bool signe);
		void	setIsLimited(bool sign);
		void	setLimitSize(unsigned int limit);
		void	setOp(Client *client, bool sign);
		void	setPassword(std::string newPassword);

		bool					getIsTopicForOp(void)const;
		bool					getInviteOnly(void)const;
		bool					getIsLimited(void)const;
		bool					getIsEmpty(void)const;
		channelMode				getChannelMode()const;
		std::string				getChannelName()const;
		std::string				getTopic()const;
		std::string				getUsers();
		std::string				getPassword()const;
		std::vector<Client *>	getInvitationList()const;
		std::map<Client*, bool> getUsersMap(void)const;
		unsigned int			getLimitSize(void)const;
		
	private:

		std::string _name; 
		
		std::map<Client*, bool> _users;
		std::vector<Client *>	_invited;
		std::string				_topic;
		std::string				_password;
		bool					_isEmpty;
		bool					_InviteOnly;
		bool					_isTopicForOp;
		bool					_isLimited;
		unsigned int			_limitSize;
		channelMode				_mode;

};

#endif
