#pragma once

#include "Client.hpp"
#include <vector>
#include <string>
#include <iostream>

class IRCServer;
class Client;

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial);
void processBuffer(IRCServer& server, int client_fd, const std::string& buffer);
void	message_server(std::string target, std::string command, Client &client, std::string message, int sd);
void	reply_server(std::string command,  Client &client, std::string message);
std::vector<std::string> tokenize(const std::string& input, char delimiter = ' ');
std::vector<std::string> split(const std::string& input, char delimiter);
void	client_connect(Client &client);
void	join(IRCServer &server, int fd, std::vector<std::string>& params);
void	find_cmd(IRCServer &server, int fd, std::vector<std::string>& params);
void	nick(IRCServer &server, int fd, std::vector<std::string>& params);
void	privmsg(IRCServer &server, int fd, std::vector<std::string>& params);
void	pass(IRCServer &server, int fd, std::vector<std::string>& params);
void	quit(IRCServer &server, int fd, std::vector<std::string>& params);
void	ping(IRCServer &server, int fd, std::vector<std::string>& params);
void	user(IRCServer &server, int fd, std::vector<std::string>& params);
void	part(IRCServer &server, int fd, std::vector<std::string>& params);
void	kick();
void	topic(IRCServer &server, int fd, std::vector<std::string>& params);
void	invite();
void	mode(IRCServer &server, int fd, std::vector<std::string>& params);
void	cap(IRCServer &server, int fd, std::vector<std::string>& params);

bool	isAlphaNum(const std::string param, int fd , IRCServer& server);
bool	isConnected(const std::string param, int fd , IRCServer& server);
bool	isValidNick(const std::string param, int fd , IRCServer& server);
bool	isValidPassword(const std::string param, int fd , IRCServer& server);
bool	isEmpty(const std::string param, int fd , IRCServer& server);
bool	isValidChannel(const std::string param, int fd , IRCServer& server);
bool	ChannelExist(const std::string param, int fd , IRCServer& server);
bool	isOnChannel(const std::string param, int fd , IRCServer& server);
bool	nickExist(const std::string param, int fd , IRCServer& server);
