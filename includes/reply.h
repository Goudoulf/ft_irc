#ifndef REPLY_H
#define REPLY_H

#define DATETIME "31/07/2024"
#include <string>
#include <map>
#include "cmds.h"

#define id(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_WELCOME(id, nickname) (":localhost 001 " + nickname + ":Welcome to the Internet Relay Network, " + nickname + "!\r\n")
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
#define RPL_STATS
enum IRCReplies {
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
	RPL_BOUNCE = 005,
	RPL_USERHOST = 302,
	RPL_ISON = 303,
	RPL_AWAY = 301,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY = 306,
	RPL_WHOISUSER = 311,
	RPL_WHOISSERVER = 312,
	RPL_WHOISOPERATOR = 313,
	RPL_WHOISIDLE = 317,
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNELS = 319,
	RPL_WHOWASUSER = 314,
	RPL_ENDOFWHOWAS = 369,
	RPL_LISTSTART = 321,
	RPL_LIST = 322,
	RPL_LISTEND = 323,
	RPL_UNIQOPIS = 325,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_SUMMONING = 342,
	RPL_INVITELIST = 346,
	RPL_ENDOFINVITELIST = 347,
	RPL_EXCEPTLIST = 348,
	RPL_ENDOFEXCEPTLIST = 349,
	RPL_VERSION = 351,
	RPL_WHOREPLY = 352,
	RPL_ENDOFWHO = 315,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_LINKS = 364,
	RPL_ENDOFLINKS = 365,
	RPL_BANLIST = 367,
	RPL_ENDOFBANLIST = 368,
	RPL_INFO = 371,
	RPL_ENDOFINFO = 374,
	RPL_MOTDSTART = 375,
	RPL_MOTD = 372,
	RPL_ENDOFMOTD = 376,
	RPL_YOUREOPER = 381,
	RPL_REHASHING = 382,
	RPL_YOURESERVICE = 383,
	RPL_TIME = 391,
	RPL_USERSSTART = 392,
	RPL_USERS = 393,
	RPL_ENDOFUSERS = 394,
	RPL_NOUSERS = 395,
	RPL_TRACELINK = 200,
	RPL_TRACECONNECTING = 201,
	RPL_TRACEHANDSHAKE = 202,
	RPL_TRACEUNKNOWN = 203,
	RPL_TRACEOPERATOR = 204,
	RPL_TRACEUSER = 205,
	RPL_TRACESERVER = 206,
	RPL_TRACESERVICE = 207,
	RPL_TRACENEWTYPE = 208,
	RPL_TRACECLASS = 209,
	RPL_TRACERECONNECT = 210,
	RPL_TRACELOG = 261,
	RPL_TRACEEND = 262,
	RPL_STATSLINKINFO = 211,
	RPL_STATSCOMMANDS = 212,
	RPL_ENDOFSTATS = 219,
	RPL_STATSUPTIME = 242,
	RPL_STATSOLINE = 243,
	RPL_UMODEIS = 221,
	RPL_SERVLIST = 234,
	RPL_SERVLISTEND = 235,
	RPL_LUSERCLIENT = 251,
	RPL_LUSEROP = 252,
	RPL_LUSERUNKNOWN = 253,
	RPL_LUSERCHANNELS = 254,
	RPL_LUSERME = 255,
	RPL_ADMINME = 256,
	RPL_ADMINLOC1 = 257,
	RPL_ADMINLOC2 = 258,
	RPL_ADMINEMAIL = 259,
	RPL_TRYAGAIN = 263,
	// More replies...
};

enum IRCErrors {
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NOTOPLEVEL = 413,
	ERR_WILDTOPLEVEL = 414,
	ERR_BADMASK = 415,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NOADMININFO = 423,
	ERR_FILEERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_UNAVAILRESOURCE = 437,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_SUMMONDISABLED = 445,
	ERR_USERSDISABLED = 446,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_YOUWILLBEBANNED = 466,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOCHANMODES = 477,
	ERR_BANLISTFULL = 478,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_RESTRICTED = 484,
	ERR_UNIQOPPRIVSNEEDED = 485,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
  // More errors...
};

void sendIRCReply(Client& client, std::string code, std::map<std::string, std::string>& params); 
#endif
