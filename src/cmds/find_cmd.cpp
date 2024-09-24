#include "../../includes/cmds.h"
#include "../../includes/debug.h"
#include "../../includes/reply.h"
#include <sstream>
#include <string>
#include <map>

const size_t MAX_BUFFER_SIZE = 512;

std::map<int, std::string> clientPartialBuffers;

void dispatchCommand(IRCServer& server, int client_fd, const std::string& command, std::vector<std::string>& params)
{
    Client* client = (server.getClients()->find(client_fd))->second;
    std::map<std::string, void(*)(IRCServer&, int, std::vector<std::string>&)> commandHandlers = {
		{"JOIN", join},
		{"NICK", nick},
		{"NOTICES", privmsg},
		{"PRIVMSG", privmsg},
		{"PASS", pass},
		{"USER", user},
		{"QUIT", quit},
		{"PING", ping},
		{"TOPIC", topic},
		// {"KICK ", &kick},
		//{"TOPIC ", &topic},
		{"PART", part},
		// {"INVITE ", &invite},
		{"MODE", mode},
		{"CAP", cap},
	};
    std::map<std::string, void(*)(IRCServer& ,int, std::vector<std::string>&)>::iterator it = commandHandlers.find(command);
    log(INFO, "cmd =" + command + "|");
    if (it != commandHandlers.end()) {
	it->second(server, client_fd, params);
    } else {
        // Handle unknown command
        log(ERROR, "Unknown IRC command: " + command);
	std::map<std::string, std::string> par {{"command", command}};
        sendIRCReply(*client, "421", par);
    }
}

void processMessage(IRCServer& server,int client_fd, const std::string& message) {
    std::string trimmedMessage = message;
    std::istringstream iss(trimmedMessage);
    std::string prefix, command, params;

    trimmedMessage.erase(0, trimmedMessage.find_first_not_of(" \r\n"));
    trimmedMessage.erase(trimmedMessage.find_last_not_of(" \r\n") + 1);

    if (trimmedMessage.empty())
        return;
    if (trimmedMessage[0] == ':'){
        iss >> prefix;  
        prefix = prefix.substr(1);
    }
    iss >> command;
    if (command.empty()) {
        log(ERROR, "Received an invalid IRC message with no command.");
        return;
    }
    else if (command == "CAP")
	return ;
    std::vector<std::string> parsedParams;
    std::string param;

    while (iss >> param)
    {
	if (param[0] == ':')
	{
	    std::string trailing;
	    std::getline(iss, trailing);
	    parsedParams.push_back(param.substr(1) + trailing);
	    break;
	} 
	parsedParams.push_back(param);
    }
    dispatchCommand(server, client_fd, command, parsedParams);
}

std::vector<std::string> splitBuffer(const std::string& buffer, std::string& remainingPartial) {
    std::vector<std::string> lines;
    std::string temp;
    std::istringstream stream(buffer);
    
    while (std::getline(stream, temp)) {
        if (!temp.empty() && temp.back() == '\r') {
            temp.pop_back();
        }
        lines.push_back(temp);
    }

    if (!buffer.empty() && buffer.back() != '\n') {
        remainingPartial = lines.back();
        lines.pop_back();
    } else {
        remainingPartial.clear();
    }
    return lines;
}

void processBuffer(IRCServer& server, int client_fd, const std::string& buffer) {
    log(DEBUG, "{" + buffer + "}");
    std::string& clientPartial = clientPartialBuffers[client_fd];

    std::string completeBuffer = clientPartial + buffer;
    if (completeBuffer.size() > MAX_BUFFER_SIZE) {
        log(ERROR, "Buffer overflow from client , disconnecting.");
        close(client_fd);
        clientPartialBuffers.erase(client_fd);
        return;
    }

    std::string remainingPartial;
    std::vector<std::string> messages = splitBuffer(completeBuffer, remainingPartial);

    clientPartialBuffers[client_fd] = remainingPartial;
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	processMessage(server, client_fd, *it);
}
