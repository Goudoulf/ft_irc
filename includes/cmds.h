#ifndef CMDS_H
#define CMDS_H

#include "../src/Client.hpp"
#include "ircserv.h"
#include <map>
#include <iostream>
#include <string>

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