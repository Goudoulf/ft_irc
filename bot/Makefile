NAME = bot
SRCS_FILE = main.cpp bot.cpp game.cpp hanghim.cpp
DIR_SRC := botsrc/
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
