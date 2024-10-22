# **************************************************************************** #
#																			   #
#														  :::	   ::::::::    #
#	 Makefile											:+:		 :+:	:+:    #
#													  +:+ +:+		  +:+	   #
#	 By: cassie <marvin@42.fr>						+#+  +:+	   +#+		   #
#												  +#+#+#+#+#+	+#+			   #
#	 Created: 2023/12/12 12:53:45 by cassie			   #+#	  #+#			   #
#	 Updated: 2023/12/12 12:54:51 by cassie			  ###	########.fr		   #
#																			   #
# **************************************************************************** #

NAME = ircserv
SRCS_FILE = Channel.cpp client_checker.cpp Client.cpp cmds/CapCommand.cpp cmds/Command.cpp\
			cmds/CommandDirector.cpp cmds/CommandDispacher.cpp cmds/InviteCommand.cpp\
			cmds/JoinCommand.cpp cmds/KickCommand.cpp cmds/ModeCommand.cpp cmds/NickCommand.cpp\
			cmds/PartCommand.cpp cmds/PassCommand.cpp cmds/PingCommand.cpp cmds/PongCommand.cpp\
			cmds/PrivmsgCommand.cpp cmds/QuitCommand.cpp cmds/TemplateBuilder.cpp cmds/TopicCommand.cpp\
			cmds/UserCommand.cpp cmds/utils/find_cmd.cpp cmds/utils/reply.cpp\
			cmds/utils/split.cpp cmds/utils/tokenize.cpp cmds/WhoCommand.cpp cmds/WhoisCommand.cpp debug/debug.cpp \
			cmds/ParamTemplate.cpp cmds/Checker.cpp cmds/NamesCommand.cpp \
			IRCServer.cpp main.cpp cmds/DefaultCommandParser.cpp cmds/ModeCommandParser.cpp

DIR_SRC := src/
DIR_OBJ := .object/

OBJS = $(patsubst %.cpp, ${DIR_OBJ}%.o, ${SRCS})
DEPS = $(patsubst %.cpp, ${DIR_OBJ}%.d, ${SRCS})
SRCS = $(addprefix ${DIR_SRC},${SRCS_FILE})

CC = c++
MD := mkdir -p
CFLAGS = -Wall -Wextra -Werror -I includes -g -MD #-std=c++98

RM = rm -rf

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o $(NAME)

${DIR_OBJ}%.o: %.cpp Makefile
	mkdir -p $(shell dirname $@)
	$(CC) ${CFLAGS} -c $< -o $@

clean:
	${RM} ${DIR_OBJ}

fclean: clean
	${RM} ${NAME}

re: fclean all


-include $(DEPS)

.PHONY: all clean fclean re
