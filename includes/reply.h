#pragma once
#include <string>

#define RPL_WELCOME(nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network, " + nickname + "!\r\n")
#define RPL_YOURHOST(nickname) (":localhost 002 " + nickname + " :Your host is NeTwerkers, running version v0.1\r\n")
#define RPL_CREATED(nickname, date) (":localhost 003 " + nickname + " This server was created " + date + "\r\n")
#define RPL_MYINFO(nickname) (":localhost 004 " + nickname + " NetTwerkers v0.1 itkol\r\n")
#define RPL_WHOISUSER(client, nickname, username, host, real_name) (":localhost 311 " + client + " " + nickname + " " + username + " " + host + " * :" + real_name + "\r\n")
#define RPL_WHOISOPERATOR(nickname) (":localhost 313 " + nickname + " :is an IRC operator"+ "\r\n")
#define RPL_WHOISIDLE(nickname, seconds, signon_time) (":localhost 317 " + nickname + " " + seconds + " " + signon_time + " :seconds idle, signon time"+ "\r\n")
#define RPL_ENDOFWHOIS(nickname) (":localhost 318 " + nickname + " :End of /WHOIS list"+ "\r\n")
#define RPL_CHANNELMODEIS(channel, modes, mode_params) (":localhost 324 " + channel + " " + modes + " " + mode_params+ "\r\n")
#define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(channel, nickname, topic) (":localhost 332 " + nickname + " " + channel + " :" + topic + "\r\n")
#define RPL_INVITING(client, nickname, channel) (":localhost 341 " + client + " " + nickname + " " + channel + "\r\n")
#define RPL_INVITED(channel, nickname, prefix) (":" + prefix + " INVITE " + nickname + " " + channel + "\r\n")
#define RPL_INVITELIST(channel, mask) (":localhost 346 " + channel + " " + mask+ "\r\n")
#define RPL_ENDOFINVITELIST(channel) (":localhost 347 " + channel + " :End of INVITELIST"+ "\r\n")
#define RPL_WHOISSERVER(nickname, server, server_info) (":localhost 312 " + nickname + " " + server + " :" + server_info+ "\r\n")
#define RPL_WHOISCHANNELS(nickname, channels) (":localhost 319 " + nickname + " :" + channels + "\r\n")
#define RPL_WHOREPLY(channel, user, host, server, nickname, flags, hopcount, real_name) (":localhost 352 " + channel + " " + user + " " + host + " " + server + " " + nickname + " " + flags + " :" + hopcount + " " + real_name+ "\r\n")
#define RPL_ENDOFWHO(name) (":localhost 315 " + name + " :End of WHO list"+ "\r\n")
#define RPL_NAMREPLY(client, channel, nicknames) (":localhost 353 " + client + " = " + channel + " :" + nicknames+ "\r\n")   
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of NAMES list"+ "\r\n")
#define RPL_YOUREOPER() (":localhost 381 :You are now an IRC operator"+ "\r\n")
#define RPL_TIME(server, time) (":localhost 391 " + server + " :" + time+ "\r\n")
#define RPL_JOIN(prefix, channel ) (":" + prefix + " JOIN " + channel + "\r\n")
#define RPL_PART(prefix, channel, message) (":" + prefix + " PART " + channel + " :" + message + "\r\n")
#define RPL_PRIVMSG(prefix, target, message) (":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n")
#define RPL_NEWNICK(prefix, nickname) (":" + prefix + " NICK :" + nickname + "\r\n")

#define ERR_NOSUCHCHANNEL(nickname, channel) (":localhost 403 " + nickname + " " +  channel + " :No such channel" + "\r\n")
#define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")
#define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " " + channel + " :Cannot join channel (+k) \r\n")
#define ERR_NOSUCHNICK(nickname) (":localhost 401 " + nickname + " :No such nick/channel\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) (":localhost 404 " + channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYCHANNELS(channel) (":localhost 405 " + channel + " :You have joined too many channels\r\n")
#define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + " :Duplicate recipients. No message delivered\r\n")
#define ERR_UNKNOWNCOMMAND( command) (":localhost 421 " + command + " :Unknown command\r\n")
//#define ERR_NONICKNAMEGIVEN() (":localhost 431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 NICK " + nickname + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nickname) (":localhost 433 NICK " + nickname + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nickname, channel) (":localhost 441 " + channel + " " + nickname + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nickname, channel) (":localhost 443 " + nickname + " " + channel + " :is already on channel\r\n")
#define ERR_NOTREGISTERED(command) (":localhost 451 " + command + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(command) (":localhost 462 " + command + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH() (":localhost 464 PASS :Password incorrect\r\n")
#define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(client, mode) (":localhost 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(channel) (":localhost 474 " + channel + " :Cannot join channel (+b)\r\n")
#define ERR_NOCHANMODES(client, channel) (":localhost 477 " + client + " " + channel + " :Channel doesn't support modes\r\n")
#define ERR_NOPRIVILEGES() (":localhost 481 :Permission Denied- You're not an IRC operator\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(client, target, modeChar, parameter, description) (":localhost 696 " + client + " " + target + " " + modeChar + " " + parameter + " :" + description + "\r\n")
#define ERR_INVALIDKEY(client, target) (":localhost 525 " + client + " " + target + " :Key is not well-formed\r\n")

void	rplSend(int fd, std::string rpl);
