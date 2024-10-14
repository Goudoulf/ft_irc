#pragma once
#include <string>

#define DATETIME "31/07/2024"

#define idd(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_WELCOME(idd, nickname) ( idd + " " + nickname + ":Welcome to the Internet Relay Network, " + nickname + "!\r\n")
#define RPL_YOURHOST(servername, version) (":localhost 002 Your host is " + servername + ", running version " + version)
#define RPL_CREATED(date) (":localhost 003 This server was created " + date)
#define RPL_MYINFO(servername, version, user_modes, channel_modes) (":localhost 004 " + servername + " " + version + " " + user_modes + " " + channel_modes)
#define RPL_BOUNCE(servername, port) (":localhost 005 Try server " + servername + ", port " + port)
#define RPL_USERHOST(reply) (":localhost 302 :" + reply)
#define RPL_ISON(nicknames) (":localhost 303 " + nicknames + " :are online")
#define RPL_AWAY(nickname, message) (":localhost 301 " + nickname + " :is away " + message)
#define RPL_UNAWAY() (":localhost 305 :You are no longer marked as being away")
#define RPL_NOWAWAY() (":localhost 306 :You have been marked as being away")
#define RPL_WHOISUSER(nickname, username, host, real_name) (":localhost 311 " + nickname + " " + username + " " + host + " * :" + real_name)
#define RPL_WHOISSERVER(nickname, server, server_info) (":localhost 312 " + nickname + " " + server + " :" + server_info)
#define RPL_WHOISOPERATOR(nickname) (":localhost 313 " + nickname + " :is an IRC operator")
#define RPL_WHOISIDLE(nickname, seconds, signon_time) (":localhost 317 " + nickname + " " + seconds + " " + signon_time + " :seconds idle, signon time")
#define RPL_ENDOFWHOIS(nickname) (":localhost 318 " + nickname + " :End of WHOIS list")
#define RPL_WHOISCHANNELS(nickname, channels) (":localhost 319 " + nickname + " :" + channels)
#define RPL_WHOWASUSER(nickname, username, host, real_name) (":localhost 314 " + nickname + " " + username + " " + host + " * :" + real_name)
#define RPL_ENDOFWHOWAS(nickname) (":localhost 369 " + nickname + " :End of WHOWAS")
#define RPL_LISTSTART() (":localhost 321 Channel :Users  Name")
#define RPL_LIST(channel, visible, topic) (":localhost 322 " + channel + " " + visible + " :" + topic)
#define RPL_LISTEND() (":localhost 323 :End of LIST")
#define RPL_CHANNELMODEIS(channel, modes, mode_params) (":localhost 324 " + channel + " " + modes + " " + mode_params)
#define RPL_UNIQOPIS(channel, nickname) (":localhost 325 " + channel + " " + nickname)
#define RPL_NOTOPIC(channel) (":localhost 331 " + channel + " :No topic is set")
#define RPL_TOPIC(channel, topic) (":localhost 332 " + channel + " :" + topic)
#define RPL_INVITING(channel, nickname) (":localhost 341 " + channel + " " + nickname)
#define RPL_SUMMONING(user) (":localhost 342 " + user + " :Summoning user to IRC")
#define RPL_INVITELIST(channel, mask) (":localhost 346 " + channel + " " + mask)
#define RPL_ENDOFINVITELIST(channel) (":localhost 347 " + channel + " :End of INVITELIST")
#define RPL_EXCEPTLIST(channel, exception_mask) (":localhost 348 " + channel + " " + exception_mask)
#define RPL_ENDOFEXCEPTLIST(channel) (":localhost 349 " + channel + " :End of EXCEPTLIST")
#define RPL_VERSION(version, debug_level, server, comments) (":localhost 351 " + version + "." + debug_level + " " + server + " :" + comments)
#define RPL_WHOREPLY(channel, user, host, server, nickname, flags, hopcount, real_name) (":localhost 352 " + channel + " " + user + " " + host + " " + server + " " + nickname + " " + flags + " :" + hopcount + " " + real_name)
#define RPL_ENDOFWHO(name) (":localhost 315 " + name + " :End of WHO list")
#define RPL_NAMREPLY(channel, nicknames) (":localhost 353 " + channel + " :" + nicknames)
#define RPL_ENDOFNAMES(channel) (":localhost 366 " + channel + " :End of NAMES list")
#define RPL_LINKS(mask, server, hopcount, server_info) (":localhost 364 " + mask + " " + server + " :" + hopcount + " " + server_info)
#define RPL_ENDOFLINKS(mask) (":localhost 365 " + mask + " :End of LINKS list")
#define RPL_BANLIST(channel, banid) (":localhost 367 " + channel + " " + banid)
#define RPL_ENDOFBANLIST(channel) (":localhost 368 " + channel + " :End of BAN list")
#define RPL_INFO(info) (":localhost 371 :" + info)
#define RPL_ENDOFINFO() (":localhost 374 :End of INFO list")
#define RPL_MOTDSTART(server) (":localhost 375 :- " + server + " Message of the day - ")
#define RPL_MOTD(message) (":localhost 372 :- " + message)
#define RPL_ENDOFMOTD() (":localhost 376 :End of MOTD command")
#define RPL_YOUREOPER() (":localhost 381 :You are now an IRC operator")
#define RPL_REHASHING(config_file) (":localhost 382 " + config_file + " :Rehashing")
#define RPL_YOURESERVICE(service_name) (":localhost 383 You are service " + service_name)
#define RPL_TIME(server, time) (":localhost 391 " + server + " :" + time)
#define RPL_USERSSTART() (":localhost 392 :UserID   Terminal  Host")
#define RPL_USERS(user) (":localhost 393 :" + user)
#define RPL_ENDOFUSERS() (":localhost 394 :End of USERS")
#define RPL_NOUSERS() (":localhost 395 :Nobody logged in")
#define RPL_TRACELINK(version, debug_level, destination, next_server) (":localhost 200 Link " + version + " " + debug_level + " " + destination + " " + next_server)
#define RPL_TRACECONNECTING(class, server) (":localhost 201 Try. " + class + " " + server)
#define RPL_TRACEHANDSHAKE(class, server) (":localhost 202 H.S. " + class + " " + server)
#define RPL_TRACEUNKNOWN(class, client_ip) (":localhost 203 ???? " + class + " [" + client_ip + "]")
#define RPL_TRACEOPERATOR(class, nick) (":localhost 204 Oper " + class + " " + nick)
#define RPL_TRACEUSER(class, nick) (":localhost 205 User " + class + " " + nick)
#define RPL_TRACESERVER(class, intS, intC, server, host) (":localhost 206 Serv " + class + " " + intS + "S " + intC + "C " + server + " " + host)
#define RPL_TRACESERVICE(class, name, type, active_type) (":localhost 207 Service " + class + " " + name + " " + type + " " + active_type)
#define RPL_TRACENEWTYPE(new_type, client_name) (":localhost 208 " + new_type + " 0 " + client_name)
#define RPL_TRACECLASS(class, count) (":localhost 209 Class " + class + " " + count)
#define RPL_TRACERECONNECT(class, server) (":localhost 210 Reconnect " + class + " " + server)
#define RPL_TRACELOG(logfile, debug_level) (":localhost 261 File " + logfile + " " + debug_level)
#define RPL_TRACEEND(server) (":localhost 262 " + server + " :End of TRACE")
#define RPL_STATSLINKINFO(linkname, sendq, sent_messages, sent_kbytes, received_messages, received_kbytes, time_open) (":localhost 211 " + linkname + " " + sendq + " " + sent_messages + " " + sent_kbytes + " " + received_messages + " " + received_kbytes + " " + time_open)
#define RPL_STATSCOMMAND(command, count, byte_count, remote_count) (":localhost 212 " + command + " " + count + " " + byte_count + " " + remote_count)
#define RPL_ENDOFSTATS(stats_letter) (":localhost 219 " + stats_letter + " :End of STATS report")
#define RPL_STATSUPTIME(days, hours, minutes, seconds) (":localhost 242 :Server Up " + days + " days " + hours + ":" + minutes + ":" + seconds)
//#define RPL_STATS
#define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")
#define ERR_BADCHANNELKEY(channel) (":localhost 475 " + channel + " :Cannot join channel (+k) \r\n")
#define ERR_NOSUCHNICK(nickname) (":localhost 401 " + nickname + " :No such nick/channel\r\n")
#define ERR_NOSUCHSERVER(server) (":localhost 402 " + server + " :No such server\r\n")
#define ERR_NOSUCHCHANNEL(channel) (":localhost 403 " + channel + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) (":localhost 404 " + channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYCHANNELS(channel) (":localhost 405 " + channel + " :You have joined too many channels\r\n")
#define ERR_WASNOSUCHNICK(nickname) (":localhost 406 " + nickname + " :There was no such nickname\r\n")
#define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + " :Duplicate recipients. No message delivered\r\n")
#define ERR_NOORIGIN() (":localhost 409 :No origin specified\r\n")
#define ERR_NORECIPIENT() (":localhost 411 :No recipient given\r\n")
#define ERR_NOTEXTTOSEND() (":localhost 412 :No text to send\r\n")
#define ERR_NOTOPLEVEL(mask) (":localhost 413 " + mask + " :No toplevel domain specified\r\n")
#define ERR_WILDTOPLEVEL(mask) (":localhost 414 " + mask + " :Wildcard in toplevel domain\r\n")
#define ERR_UNKNOWNCOMMAND(command) (":localhost 421 " + command + " :Unknown command\r\n")
#define ERR_NOMOTD() (":localhost 422 :MOTD File is missing\r\n")
#define ERR_NOADMININFO(server) (":localhost 423 " + server + " :No administrative info available\r\n")
#define ERR_NONICKNAMEGIVEN() (":localhost 431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 " + nickname + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nickname) (":localhost 433 " + nickname + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nickname, channel) (":localhost 441 " + nickname + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nickname, channel) (":localhost 443 " + nickname + " " + channel + " :is already on channel\r\n")
#define ERR_NOLOGIN(nickname) (":localhost 444 " + nickname + " :User not logged in\r\n")
#define ERR_SUMMONDISABLED() (":localhost 445 :SUMMON has been disabled\r\n")
#define ERR_USERSDISABLED() (":localhost 446 :USERS has been disabled\r\n")
#define ERR_NOTREGISTERED() (":localhost 451 :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED() (":localhost 462 :You may not reregister\r\n")
#define ERR_NOPERMFORHOST() (":localhost 463 :Your host isn't among the privileged\r\n")
#define ERR_PASSWDMISMATCH() (":localhost 464 :Password incorrect\r\n")
#define ERR_YOUREBANNEDCREEP() (":localhost 465 :You are banned from this server\r\n")
#define ERR_CHANNELISFULL(channel) (":localhost 471 " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(mode) (":localhost 472 " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(channel) (":localhost 473 " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(channel) (":localhost 474 " + channel + " :Cannot join channel (+b)\r\n")
#define ERR_NOPRIVILEGES() (":localhost 481 :Permission Denied- You're not an IRC operator\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) (":localhost 482 " + channel + " :You're not channel operator\r\n")
#define ERR_CANTKILLSERVER() (":localhost 483 :You can't kill a server!\r\n")
#define ERR_NOOPERHOST() (":localhost 491 :No O-lines for your host\r\n")

void	rpl_send(int fd, std::string rpl);
