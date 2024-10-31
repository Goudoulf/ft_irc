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

SRCS_FILE = Channel.cpp Client.cpp debug/debug.cpp \
			IRCServer.cpp main.cpp 

CMD_FILE = CapCommand.cpp JoinCommand.cpp  NamesCommand.cpp  PassCommand.cpp \
		   PrivmsgCommand.cpp UserCommand.cpp KickCommand.cpp \
		   NickCommand.cpp PingCommand.cpp QuitCommand.cpp WhoCommand.cpp \
		   InviteCommand.cpp ModeCommand.cpp PartCommand.cpp PongCommand.cpp \
		   TopicCommand.cpp WhoisCommand.cpp

TEMPLATE_FILE = Checker.cpp CommandDirector.cpp ParamTemplate.cpp \
				TemplateBuilder.cpp initCommandTemplate.cpp

PARSER_FILE = DefaultCommandParser.cpp JoinCommandParser.cpp \
			  KickCommandParser.cpp ModeCommandParser.cpp

UTILS_FILE = getTime.cpp reply.cpp splitBuffer.cpp split.cpp

DIR_SRC := src/
DIR_CMD := src/command/
DIR_TEMPLATE := src/command/template/
DIR_PARSER := src/command/parser/
DIR_UTILS:= src/command/utils/
DIR_OBJ := .object/

OBJS = $(patsubst %.cpp, ${DIR_OBJ}%.o, ${SRCS})
DEPS = $(patsubst %.cpp, ${DIR_OBJ}%.d, ${SRCS})
SRCS = $(addprefix ${DIR_SRC},${SRCS_FILE})
SRCS += $(addprefix ${DIR_CMD},${CMD_FILE})
SRCS += $(addprefix ${DIR_TEMPLATE},${TEMPLATE_FILE})
SRCS += $(addprefix ${DIR_PARSER},${PARSER_FILE})
SRCS += $(addprefix ${DIR_UTILS},${UTILS_FILE})

CC = c++
MD := mkdir -p
CFLAGS = -Wall -Wextra -Werror -I includes -g -MD -std=c++98

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
