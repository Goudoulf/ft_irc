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

SRCS_FILE = main.cpp IRCServer.cpp Client.cpp client_checker.cpp \
			cmds/find_cmd.cpp cmds/join.cpp cmds/nick.cpp cmds/pass.cpp cmds/ping.cpp\
			cmds/privmsg.cpp cmds/quit.cpp cmds/mode.cpp cmds/client_connect.cpp Channel.cpp\
			cmds/reply.cpp cmds/tokenize.cpp cmds/split.cpp debug/debug.cpp cmds/part.cpp\
			cmds/topic.cpp cmds/user.cpp reply2.cpp cmds/cap.cpp \
			test/cmd/CapCommand.cpp test/cmd/Command.cpp test/cmd/CommandDirector.cpp \
			test/cmd/CommandDispacher.cpp test/cmd/InviteCommand.cpp test/cmd/JoinCommand.cpp \
			test/cmd/KickCommand.cpp test/cmd/ModeCommand.cpp test/cmd/NickCommand.cpp \
			test/cmd/PartCommand.cpp test/cmd/PassCommand.cpp test/cmd/PingCommand.cpp\
			test/cmd/PongCommand.cpp test/cmd/PrivmsgCommand.cpp test/cmd/QuitCommand.cpp \
			test/cmd/TemplateBuilder.cpp test/cmd/TopicCommand.cpp\
			test/cmd/UserCommand.cpp test/cmd/WhoCommand.cpp test/cmd/WhoisCommand.cpp

DIR_SRC := src/
DIR_OBJ := .object/

OBJS = $(patsubst %.cpp, ${DIR_OBJ}%.o, ${SRCS})
DEPS = $(patsubst %.cpp, ${DIR_OBJ}%.d, ${SRCS})
SRCS = $(addprefix ${DIR_SRC},${SRCS_FILE})

CC = c++
MD := mkdir -p
CFLAGS = -Wall -Wextra -Werror -g -MD #-std=c++98

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
