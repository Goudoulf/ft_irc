#ifndef CMDS_H
#define CMDS_H

#include "../src/Client.hpp"
#include "ircserv.h"
#include <map>
#include <iostream>
#include <string>

class IRCServer;
class Client;

void	message_server(std::string target, std::string command, Client &client, std::string message, int sd);
void	reply_server(std::string command,  Client &client, std::string message);
std::vector<std::string> tokenize(const std::string& input, char delimiter = ' ');
std::vector<std::string> split(const std::string& input, char delimiter);
void	client_connect(Client &client);
void	join(IRCServer &server, int fd, const std::vector<std::string>& params);
void	find_cmd(IRCServer &server, int fd, std::vector<std::string>& params);
void	join(IRCServer &server, int fd, const std::vector<std::string>& params);
void	nick(IRCServer &server, int fd, const std::vector<std::string>& params);
void	privmsg(IRCServer &server, int fd, const std::vector<std::string>& params);
void	pass(IRCServer &server, int fd, const std::vector<std::string>& params);
void	quit(IRCServer &server, int fd, const std::vector<std::string>& params);
void	ping(IRCServer &server, int fd, const std::vector<std::string>& params);
void	part(IRCServer &server, int fd, const std::vector<std::string>& params);
void	kick();
void	topic(IRCServer &server, int fd, const std::vector<std::string>& params);
void	invite();
void	mode(IRCServer &server, int fd, const std::vector<std::string>& params);


#endif
