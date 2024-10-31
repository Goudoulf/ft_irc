# TODO

## LEO

- check les sends dans les commandes ou des erreurs ect
- faire un coup de clean et SUPRIMER les fichier vide
- test commande decouper ajouter dans client clientpartialbuffer
- Gestion d'exit propre init server
- check les leaks

## ROMAIN

- test toutes les commandes, avec checker et erreurs les reply

> Password
>	rpl :	#OK ERR_NEEDMOREPARAM		#ERR_ALREADYREGISTRED

Nickname
	rpl :	ERR_NONICKNAMEGIVEN		ERR_ERRONEUSNICKNAME
			(ERR_NICKNAMEINUSE)		ERR_NICKCOLLISION

User
	rpl :	ERR_NEEDMOREPARAMS		ERR_ALREADYREGISTRED

Quit
	rpl :	None

Join
	rpl :	ERR_NEEDMOREPARAMS		ERR_BANNEDFROMCHAN
			ERR_INVITEONLYCHAN		ERR_BADCHANNELKEY
			ERR_CHANNELISFULL		ERR_BADCHANMASK
			ERR_NOSUCHCHANNEL		ERR_TOOMANYCHANNELS
			RPL_TOPIC

Part
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHCHANNEL
			ERR_NOTONCHANNEL

Mode
	rpl :	ERR_NEEDMOREPARAMS		RPL_CHANNELMODEIS
			ERR_CHANOPRIVSNEEDED	ERR_NOSUCHNICK
			ERR_NOTONCHANNEL		ERR_KEYSET
			RPL_BANLIST				RPL_ENDOFBANLIST
			ERR_UNKNOWNMODE			ERR_NOSUCHCHANNEL

			ERR_USERSDONTMATCH		RPL_UMODEIS
			ERR_UMODEUNKNOWNFLAG

Topic
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOTONCHANNEL
			RPL_NOTOPIC				RPL_TOPIC
			ERR_CHANOPRIVSNEEDED

Names
	rpl :	RPL_NAMREPLY			RPL_ENDOFNAMES

Invite
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHNICK
			ERR_NOTONCHANNEL		ERR_USERONCHANNEL
			ERR_CHANOPRIVSNEEDED
			RPL_INVITING			RPL_AWAY

Kick
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHCHANNEL
			ERR_BADCHANMASK			ERR_CHANOPRIVSNEEDED
			ERR_NOTONCHANNEL

Ping
	rpl :	ERR_NOORIGIN			ERR_NOSUCHSERVER

Pong
	rpl :	ERR_NOORIGIN			ERR_NOSUCHSERVER

Privmsg
	rpl :	ERR_NORECIPIENT			ERR_NOTEXTTOSEND
			ERR_CANNOTSENDTOCHAN	ERR_NOTOPLEVEL
			ERR_WILDTOPLEVEL		ERR_TOOMANYTARGETS
			ERR_NOSUCHNICK
			RPL_AWAY

Who
	rpl :	ERR_NOSUCHSERVER
			RPL_WHOREPLY			RPL_ENDOFWHO

Whois
	rpl :	ERR_NOSUCHSERVER		ERR_NONICKNAMEGIVEN
			RPL_WHOISUSER			RPL_WHOISCHANNELS
			RPL_WHOISCHANNELS		RPL_WHOISSERVER
			RPL_AWAY				RPL_WHOISOPERATOR
			RPL_WHOISIDLE			ERR_NOSUCHNICK
			RPL_ENDOFWHOIS

- test nc : envoi partiel, ctrl c ctrl d ctrl z test quelque commande avec cas chelou
- check les leaks
- dictionary check validity
- check nombre de joueur creation de je

## CAMILLE


- Clean les logs, et les message debug rajoute logi, rajouter log main et server et autre.
-  OK : Finir le main, : gestion de mot de passe , port et du debug
- Input bot port et mot de passe check parsing
- clean les includes et ifndef
- test main input port password ect;
- check les leaks

- check bot parsing et command et gestion erreurs
- check mini jeu
- add log system bot
- Camel case
- Makefile clean
