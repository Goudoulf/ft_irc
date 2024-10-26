#include "CommandDirector.hpp"
#include "TemplateBuilder.hpp"
#include "JoinCommand.hpp"
#include "JoinCommandParser.hpp"
#include "PartCommand.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"
#include "NickCommand.hpp"
#include "QuitCommand.hpp"
#include "ModeCommand.hpp"
#include "ModeCommandParser.hpp"
#include "TopicCommand.hpp"
#include "InviteCommand.hpp"
#include "KickCommand.hpp"
#include "KickCommandParser.hpp"
#include "PrivmsgCommand.hpp"
#include "NamesCommand.hpp"
#include "WhoCommand.hpp"
#include "WhoisCommand.hpp"
#include "PingCommand.hpp"
#include "PongCommand.hpp"
#include "CapCommand.hpp"

void    setCommandTemplate(CommandDirector *director)
{
    director->addCommand(TemplateBuilder::Builder()
                          .name("CAP")
                          .level(NONE)
                          .param("toto", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new CapCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("PASS")
                          .level(NONE)
                          .param("password", ParamTemplate::Builder()
                                 .addChecker(&isConnected)
                                 .addChecker(&isValidPassword)
                                 .build()
                                 )
                          .command(new PassCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("NICK")
                          .level(CONNECTED)
                          .param("nick", ParamTemplate::Builder()
                                 .addChecker(&isEmpty) 
                                 .addChecker(&isValidNick)
                                 .build()
                                 )
                          .command(new NickCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("USER")
                          .level(CONNECTED)
                          .param("user", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("mode", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("unused", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("realname", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new UserCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("JOIN")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&isValidChannel)
                                 .addChecker(&isInLimits)
                                 .addChecker(&isInvited)
                                 .build()
                                 )
                          .param("key", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new JoinCommand())
                          .parser(new JoinCommandParser())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("PART")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .build()
                                 )
                          .trailing("comment", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new PartCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("QUIT")
                          .level(REGISTERED)
                          .trailing("message",  ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new QuitCommand())
                          .build()
                          );
    
    director->addCommand(TemplateBuilder::Builder()
                          .name("MODE")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .addChecker(&isOp)
                                 .build()
                                 )
                          .param("modes",  ParamTemplate::Builder()
                                 .addChecker(&isValidMode)
                                 .isOptional()
                                 .build()
                                 )
                          .command(new ModeCommand())
                          .parser(new ModeCommandParser())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("TOPIC")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .build()
                                 )
                          .trailing("topic", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new TopicCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("INVITE")
                          .level(REGISTERED)
                          .param("nickname", ParamTemplate::Builder()
                                 .addChecker(&nickExist)
                                 .build()
                                 )
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&isValidInvite)
                                 .build()
                                 )
                          .command(new InviteCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("KICK")
                          .level(REGISTERED)
                          .param("channel", ParamTemplate::Builder()
                                 .addChecker(&ChannelExist)
                                 .addChecker(&isOnChannel)
                                 .addChecker(&isOp)
                                 .build()
                                 )
                          .param("user", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("comment", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new KickCommand())
                          .parser(new KickCommandParser())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("PRIVMSG")
                          .level(REGISTERED)
                          .param("msgtarget", ParamTemplate::Builder()
                                 .build()
                                 )
                          .trailing("message",  ParamTemplate::Builder()
                                 .build()
                                 )

                          .command(new PrivmsgCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("WHO")
                          .level(REGISTERED)
                          .param("mask", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .param("o", ParamTemplate::Builder()
                                 .isOptional()
                                 .build()
                                 )
                          .command(new WhoCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("WHOIS")
                          .level(REGISTERED)
                          .param("target", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("mask", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new WhoisCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("PING")
                          .level(REGISTERED)
                          .param("server1", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("server2", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new PingCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("PONG")
                          .level(REGISTERED)
                          .param("server1", ParamTemplate::Builder()
                                 .build()
                                 )
                          .param("server2", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new PongCommand())
                          .build()
                          );

    director->addCommand(TemplateBuilder::Builder()
                          .name("NAMES")
                          .level(REGISTERED)
                          .param("channels", ParamTemplate::Builder()
                                 .build()
                                 )
                          .command(new NamesCommand())
                          .build()
                          );
}
