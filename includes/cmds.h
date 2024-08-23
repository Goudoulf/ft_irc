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
void	find_cmd(Client &client, IRCServer &server);
void	join(Client &client, IRCServer &server);
void	nick(Client &client, IRCServer &server);
void	privmsg(Client &client, IRCServer &server);
void	pass(Client &client, IRCServer &server);
void	quit(Client &client, IRCServer &server);
void	kick();
void	topic();
void	part();
void	invite();
void	mode();


#endif
