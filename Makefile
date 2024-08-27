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
			cmds/find_cmd.cpp cmds/join.cpp cmds/nick.cpp cmds/pass.cpp\
			cmds/privmsg.cpp cmds/quit.cpp cmds/client_connect.cpp Channel.cpp\
			cmds/reply.cpp cmds/tokenize.cpp cmds/split.cpp debug/debug.cpp cmds/part.cpp

DIR_SRC := src/
DIR_OBJ := .object/

OBJS = $(patsubst %.cpp, ${DIR_OBJ}%.o, ${SRCS})
DEPS = $(patsubst %.cpp, ${DIR_OBJ}%.d, ${SRCS})
SRCS = $(addprefix ${DIR_SRC},${SRCS_FILE})

CC = c++
MD := mkdir -p
CFLAGS = -Wall -Wextra -Werror -g -std=c++98 -MD

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
