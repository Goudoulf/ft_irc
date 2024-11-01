# TODO

## LEO

- OK check les sends dans les commandes ou des erreurs ect
- OK check les leaks
- Input bot port et mot de passe check parsing
- check bot parsing et command et gestion erreurs
jsp pb qui fait qu'on recoit le RPL 266 trop tot parfois donc des fois on a "@bot" des fois "@bot\r\n" des fois "@bot "

## ROMAIN

- test toutes les commandes, avec checker et erreurs les reply

>Nickname
	rpl :	ERR_NONICKNAMEGIVEN(n'est pas utilise sur Nick)

>Mode (update list OP apres +o) add Name_reply
	rpl :	ERR_NOSUCHNICK (pas utilise dans MODE) a utilise a la place de Not on channel

Invite
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHNICK
			ERR_NOTONCHANNEL		ERR_USERONCHANNEL
			ERR_CHANOPRIVSNEEDED
			RPL_INVITING

Kick
	rpl :	ERR_NEEDMOREPARAMS		ERR_NOSUCHCHANNEL
			ERR_BADCHANMASK			ERR_CHANOPRIVSNEEDED
			ERR_NOTONCHANNEL

Privmsg
	rpl :	ERR_NORECIPIENT			ERR_NOTEXTTOSEND
			ERR_CANNOTSENDTOCHAN	ERR_NOTOPLEVEL
			ERR_WILDTOPLEVEL		ERR_TOOMANYTARGETS
			ERR_NOSUCHNICK

Whois
	rpl :	ERR_NOSUCHSERVER		ERR_NONICKNAMEGIVEN
			RPL_WHOISUSER			RPL_WHOISCHANNELS
			RPL_WHOISCHANNELS		RPL_WHOISSERVER
			RPL_AWAY				RPL_WHOISOPERATOR
			RPL_WHOISIDLE			ERR_NOSUCHNICK
			RPL_ENDOFWHOIS

- test nc : envoi partiel, ctrl c ctrl d ctrl z test quelque commande avec cas chelou
	erreur ? avec echo "PASS 123456\r\nNICK test\r\nUSER a a a :a\r\nJOIN #42\r\nPRIVMSG #42 :the cake is a lie\r\nQUIT\n\rJOIN #43\n\rJOIN #42" | nc 127.0.0.1 6665
	arrive a join sans \r\n (pareil pour toutes les commandes ?)
- check les leaks
- dictionary check validity -----> compter le nombre de ligne du dico
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
- Makefile clean
