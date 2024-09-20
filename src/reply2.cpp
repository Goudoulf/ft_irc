/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:15:45 by cassie            #+#    #+#             */
/*   Updated: 2024/09/06 10:30:56 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <utility>
#include "../includes/cmds.h"
#include "IRCServer.hpp"

typedef struct s_reply
{
	std::map<std::string, std::string> ReplyTemplates = {
		{"001", "Welcome to the Internet Relay Network, {nickname}!"},
		{"002", "Your host is {servername}, running version {version}"},
		{"003", "This server was created {date}"},
		{"004", "{servername} {version} {user modes} {channel modes}"},
		{"005", "Try server {servername}, port {port}"},
		{"302", ":{reply}"},
		{"303", "{nicknames} :are online"},
		{"301", "{nickname} :is away {message}"},
		{"305", ":You are no longer marked as being away"},
		{"306", ":You have been marked as being away"},
		{"311", "{nickname} {username} {host} * :{real name}"},
		{"312", "{nickname} {server} :{server info}"},
		{"313", "{nickname} :is an IRC operator"},
		{"317", "{nickname} {seconds} {signon time} :seconds idle, signon time"},
		{"318", "{nickname} :End of WHOIS list"},
		{"319", "{nickname} :{channels}"},
		{"314", "{nickname} {username} {host} * :{real name}"},
		{"369", "{nickname} :End of WHOWAS"},
		{"321", "Channel :Users  Name"},
		{"322", "{channel} {visible} :{topic}"},
		{"323", ":End of LIST"},
		{"325", "{channel} {nickname}"},
		{"324", "{channel} {modes} {mode params}"},
		{"331", "{channel} :No topic is set"},
		{"332", "{channel} :{topic}"},
		{"341", "{channel} {nickname}"},
		{"342", "{user} :Summoning user to IRC"},
		{"346", "{channel} {mask}"},
		{"347", "{channel} :End of INVITELIST"},
		{"348", "{channel} {exception mask}"},
		{"349", "{channel} :End of EXCEPTLIST"},
		{"351", "{version}.{debug level} {server} :{comments}"},
		{"352", "{channel} {user} {host} {server} {nickname} {H|G}[*][@|+] :{hopcount} {real name}"},
		{"315", "{name} :End of WHO list"},
		{"353", "{channel} :{nicknames}"},
		{"366", "{channel} :End of NAMES list"},
		{"364", "{mask} {server} :{hopcount} {server info}"},
		{"365", "{mask} :End of LINKS list"},
		{"367", "{channel} {banid}"},
		{"368", "{channel} :End of BAN list"},
		{"371", ":{info}"},
		{"374", ":End of INFO list"},
		{"375", ":- {server} Message of the day - "},
		{"372", ":- {message}"},
		{"376", ":End of MOTD command"},
		{"381", ":You are now an IRC operator"},
		{"382", "{config file} :Rehashing"},
		{"383", "You are service {service name}"},
		{"391", "{server} :{time}"},
		{"392", ":UserID   Terminal  Host"},
		{"393", ":{user}"},
		{"394", ":End of USERS"},
		{"395", ":Nobody logged in"},
		{"200", "Link {version} {debug level} {destination} {next server}"},
		{"201", "Try. {class} {server}"},
		{"202", "H.S. {class} {server}"},
		{"203", "???? {class} [{client IP address in dot form}]"},
		{"204", "Oper {class} {nick}"},
		{"205", "User {class} {nick}"},
		{"206", "Serv {class} {intS}S {intC}C {server} {nick!user|*!*}@{host|server}"},
		{"207", "Service {class} {name} {type} {active type}"},
		{"208", "{new type} 0 {client name}"},
		{"209", "Class {class} {count}"},
		{"210", "Reconnect {class} {server}"},
		{"261", "File {logfile} {debug level}"},
		{"262", "{server} :End of TRACE"},
		{"211", "{linkname} {sendq} {sent messages} {sent Kbytes} {received messages} {received Kbytes} {time open}"},
		{"212", "{command} {count} {byte count} {remote count}"},
		{"219", "{stats letter} :End of STATS report"},
		{"242", ":Server Up {days} days {hours}:{minutes}:{seconds}"},
		{"243", "O {hostmask} * {name}"},
		{"221", "{user mode string}"},
		{"234", "{name} {server} {mask} {type} {hopcount} {info}"},
		{"235", "{mask} :End of service listing"},
		{"251", ":There are {user count} users and {invisible count} invisible on {server count} servers"},
		{"252", "{count} :operator(s) online"},
		{"253", "{count} :unknown connection(s)"},
		{"254", "{count} :channels formed"},
		{"255", ":I have {user count} clients and {server count} servers"},
		{"256", "{server} :Administrative info"},
		{"257", ":{admin info line 1}"},
		{"258", ":{admin info line 2}"},
		{"259", ":{admin info line 3}"},
		{"263", ":Please wait a while and try again."},
		{"401", "{nickname} :No such nick/channel"},
		{"402", "{server name} :No such server"},
		{"403", "{channel name} :No such channel"},
		{"404", "{channel name} :Cannot send to channel"},
		{"405", "{channel name} :You have joined too many channels"},
		{"406", "{nickname} :There was no such nickname"},
		{"407", "{target} :{error code} recipients. {abort message}"},
		{"408", "{service name} :No such service"},
		{"409", ":No origin specified"},
		{"411", ":No recipient given ({command})"},
		{"412", ":No text to send"},
		{"413", "{mask} :No toplevel domain specified"},
		{"414", "{mask} :Wildcard in toplevel domain"},
		{"415", "{mask} :Bad Server/host mask"},
		{"421", "{command} :Unknown command"},
		{"422", ":MOTD File is missing"},
		{"423", "{server} :No administrative info available"},
		{"424", ":File error doing {file op} on {file}"},
		{"431", ":No nickname given"},
		{"432", "{nick} :Erroneous nickname"},
		{"433", "{nick} :Nickname is already in use"},
		{"436", "{nick} :Nickname collision KILL from {user}@{host}"},
		{"437", "{nick/channel} :Nick/channel is temporarily unavailable"},
		{"441", "{nick} {channel} :They aren't on that channel"},
		{"442", "{channel} :You're not on that channel"},
		{"443", "{user} {channel} :is already on channel"},
		{"444", "{user} :User not logged in"},
		{"445", ":SUMMON has been disabled"},
		{"446", ":USERS has been disabled"},
		{"451", ":You have not registered"},
		{"461", "{command} :Not enough parameters"},
		{"462", ":Unauthorized command (already registered)"},
		{"463", ":Your host isn't among the privileged"},
		{"464", ":Password incorrect"},
		{"465", ":You are banned from this server"},
		{"466", ":You will be banned"},
		{"467", "{channel} :Channel key already set"},
		{"471", "{channel} :Cannot join channel (+l)"},
		{"472", "{char} :is unknown mode char to me for {channel}"},
		{"473", "{channel} :Cannot join channel (+i)"},
		{"474", "{channel} :Cannot join channel (+b)"},
		{"475", "{channel} :Cannot join channel (+k)"},
		{"476", "{channel} :Bad Channel Mask"},
		{"477", "{channel} :Channel doesn't support modes"},
		{"478", "{channel} {char} :Channel list is full"},
		{"481", ":Permission Denied- You're not an IRC operator"},
		{"482", "{channel} :You're not channel operator"},
		{"483", ":You can't kill a server!"},
		{"484", ":Your connection is restricted!"},
		{"485", ":You're not the original channel operator"},
		{"491", ":No O-lines for your host"},
		{"501", ":Unknown MODE flag"},
		{"502", ":Cannot change mode for other users"},
	};
}t_Reply;

std::map<std::string, std::string> insert_default_value(Client &client, std::map<std::string, std::string>& params)
{
    params.insert(std::pair<std::string, std::string>({"servername", "NetTwerkers"}));
    params.insert(std::pair<std::string, std::string>({"version", "v0.1"}));
    params.insert(std::pair<std::string, std::string>({"user modes", "itkol"}));
    params.insert(std::pair<std::string, std::string>({"channel modes", "itkol"}));
    params.insert(std::pair<std::string, std::string>({"port", client.getServer()->getPort()}));
    params.insert(std::pair<std::string, std::string>({"date", client.getServer()->getCreationDate()}));
    return (params);
}


void sendIRCReply(Client& client, std::string code, std::map<std::string, std::string>& params) {
    t_Reply reply;
    params = insert_default_value(client, params);
   std::map<std::string, std::string>::iterator it = reply.ReplyTemplates.find(code);
    if (it != reply.ReplyTemplates.end()) {
        std::string message = it->second;
        
        // Replace placeholders with actual parameter values
        for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); it++) {
            std::string placeholder = "{" + it->first + "}";
            size_t pos;
            while ((pos = message.find(placeholder)) != std::string::npos) {
                message.replace(pos, placeholder.length(), it->second);
            }
        }
	std::string rep = ":127.0.0.1 " + code + " " + client.GetNickname() + " " + message + "\n\r";
	send(client.GetSocket(), rep.c_str(), rep.size(), 0);

    } else {
        // Handle unknown reply code
        std::cout << "Unknown IRC code: " << code << std::endl;
    }
}

