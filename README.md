# TODO

## LEO

- OK check les sends dans les commandes ou des erreurs ect
- OK check les leaks
- Input bot port et mot de passe check parsing
- check bot parsing et command et gestion erreurs

## ROMAIN

- test toutes les commandes, avec checker et erreurs les reply

edit servername au debut des reply

> Password
	rpl :	~~OK ERR_NEEDMOREPARAM~~		~~ERR_ALREADYREGISTRED~~

>Nickname
	rpl :	ERR_NONICKNAMEGIVEN(n'est pas utilise sur Nick)		~~ERR_ERRONEUSNICKNAME~~
			~~ERR_NICKNAMEINUSE~~

>User
	rpl :	~~ERR_NEEDMOREPARAMS~~		ERR_ALREADYREGISTRED (Potentiel probleme : Si j'utilise USER sans arg, est ce que je dois avoir needmoreparam ou alreadyregistred apres m'etre enregistre)

>Quit
	rpl :	None

>Join (Le message "Now talking on channel" revient a chaque fois qu'on join un channel dans lequel on se trouve)
	rpl :	~~ERR_NEEDMOREPARAMS~~
			~~ERR_INVITEONLYCHAN~~		~~ERR_BADCHANNELKEY~~
			~~ERR_CHANNELISFULL~~
			ERR_NOSUCHCHANNEL (doute sur les espaces et les surplus de parametres)		ERR_TOOMANYCHANNELS (pas utile ?)
			~~RPL_TOPIC~~

>Part (pas de gestion de plusieurs channel)
	rpl :	~~ERR_NEEDMOREPARAMS~~		~~ERR_NOSUCHCHANNEL~~
			~~ERR_NOTONCHANNEL~~

>Mode (update OP apres +o)
	rpl :	~~ERR_NEEDMOREPARAMS~~		~~RPL_CHANNELMODEIS~~
			~~ERR_CHANOPRIVSNEEDED~~	ERR_NOSUCHNICK (pas utilise dans MODE)
			~~ERR_NOTONCHANNEL~~
			~~ERR_UNKNOWNMODE~~			~~ERR_NOSUCHCHANNEL~~

>Topic
	rpl :	~~ERR_NEEDMOREPARAMS~~		~~ERR_NOTONCHANNEL~~
			~~RPL_NOTOPIC~~				~~RPL_TOPIC~~
			~~ERR_CHANOPRIVSNEEDED~~

Names
	rpl :	~~RPL_NAMREPLY~~			~~RPL_ENDOFNAMES~~

Invite
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHNICK
			ERR_NOTONCHANNEL		ERR_USERONCHANNEL
			ERR_CHANOPRIVSNEEDED
			RPL_INVITING

Kick
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHCHANNEL
			ERR_BADCHANMASK			ERR_CHANOPRIVSNEEDED
			ERR_NOTONCHANNEL

Ping
	rpl :	ERR_NOORIGIN

Pong
	rpl :	ERR_NOORIGIN

Privmsg
	rpl :	ERR_NORECIPIENT			ERR_NOTEXTTOSEND
			ERR_CANNOTSENDTOCHAN	ERR_NOTOPLEVEL
			ERR_WILDTOPLEVEL		ERR_TOOMANYTARGETS
			ERR_NOSUCHNICK

(Who
	rpl :	ERR_NOSUCHSERVER
			RPL_WHOREPLY			RPL_ENDOFWHO

Whois
	rpl :	ERR_NOSUCHSERVER		ERR_NONICKNAMEGIVEN
			RPL_WHOISUSER			RPL_WHOISCHANNELS
			RPL_WHOISCHANNELS		RPL_WHOISSERVER
			RPL_AWAY				RPL_WHOISOPERATOR
			RPL_WHOISIDLE			ERR_NOSUCHNICK
			RPL_ENDOFWHOIS)

- test nc : envoi partiel, ctrl c ctrl d ctrl z test quelque commande avec cas chelou
	erreur ? avec echo "PASS 123456\r\nNICK test\r\nUSER a a a :a\r\nJOIN #42\r\nPRIVMSG #42 :the cake is a lie\r\nQUIT\n\rJOIN #43\n\rJOIN #42" | nc 127.0.0.1 6665
	arrive a join sans \r\n (pareil pour toutes les commandes ?)
- check les leaks
- dictionary check validity
- check nombre de joueur creation de je

## CAMILLE


- Clean les logs, et les message debug rajoute logi, rajouter log main et server et autre.
- OK : Finir le main, : gestion de mot de passe , port et du debug
- OK clean les includes et ifndef
- OK test main input port password ect;
- OK Makefile clean

- check mini jeu
- add log system bot
- Camel case


check ca : .param("toto", ParamTemplate::Builder()
ping ?
autre nom pour les fonctions de quit
