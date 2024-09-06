#include "../../includes/cmds.h"
#include "../../includes/debug.h"

void	map_init(std::map<std::string, void (*)(Client&, IRCServer&)> &map_func)
{
	map_func.insert(std::make_pair("JOIN ", &join));
	map_func.insert(std::make_pair("NICK ", &nick));
	map_func.insert(std::make_pair("NOTICES ", &privmsg));
	map_func.insert(std::make_pair("PRIVMSG ", &privmsg));
	map_func.insert(std::make_pair("PASS ", &pass));
	map_func.insert(std::make_pair("QUIT ", &quit));
	map_func.insert(std::make_pair("PING ", &ping));
	map_func.insert(std::make_pair("TOPIC ", &topic));
	// map_func.insert(std::make_pair("KICK ", &kick));
	//map_func.insert(std::make_pair("TOPIC ", &topic));
	map_func.insert(std::make_pair("PART ", &part));
	// map_func.insert(std::make_pair("INVITE ", &invite));
	map_func.insert(std::make_pair("MODE ", &mode));
}

void	find_cmd(Client &client, IRCServer &server)
{
	log(INFO, "find cmd"); 
	std::string buf = client.GetBufferString();
	std::string bufe = "buffer[\n" + client.GetBufferString() + "]";
	log(DEBUG,bufe); 
	std::map<std::string, void (*)(Client&, IRCServer&)> map_func;
	map_init(map_func);
	for (std::map<std::string, void (*)(Client&, IRCServer&)>::iterator it = map_func.begin(); it != map_func.end(); it++)
		if (buf.compare(0, it->first.length(), it->first) == 0)
			it->second(client, server);
}
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

void processIncomingMessage(int client_fd, const std::string& message) {
    // Trim whitespace from the message
    std::string trimmedMessage = message;
    trimmedMessage.erase(0, trimmedMessage.find_first_not_of(" \r\n"));
    trimmedMessage.erase(trimmedMessage.find_last_not_of(" \r\n") + 1);

    if (trimmedMessage.empty()) {
        return;
    }

    std::istringstream iss(trimmedMessage);
    std::string prefix, command, params;

    if (trimmedMessage[0] == ':') {
        iss >> prefix;  
        prefix = prefix.substr(1);
    }
    iss >> command;
    if (command.empty()) {
        log(ERROR, "Received an invalid IRC message with no command.");
        return;
    }
    std::unordered_map<std::string, std::string> parsedParams;
    std::string param;
    bool trailingStarted = false;

    while (iss >> param) {
        if (param[0] == ':') {
            // If we encounter a ':', this marks the start of the trailing parameter
            trailingStarted = true;
            std::string trailing;
            std::getline(iss, trailing);
            parsedParams["trailing"] = param.substr(1) + trailing;  // Combine ':' and the rest
            break;
        } else if (!trailingStarted) {
            // Handle normal parameters
            if (command == "NICK") {
                parsedParams["nickname"] = param;
            } else if (command == "USER") {
                if (parsedParams.find("username") == parsedParams.end()) {
                    parsedParams["username"] = param;
                } else if (parsedParams.find("hostname") == parsedParams.end()) {
                    parsedParams["hostname"] = param;
                } else if (parsedParams.find("servername") == parsedParams.end()) {
                    parsedParams["servername"] = param;
                } else {
                    parsedParams["realname"] = param;
                }
            }
            // Add more parameter handling for other commands...
        }
    }

    // Ensure server name is included in the parameters (custom field)
    parsedParams["servername"] = "irc.example.com";  // Replace with your server's name

    // Call the command dispatcher to handle the command
    dispatchCommand(client_fd, command, parsedParams);
}
