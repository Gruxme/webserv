NAME := webserv
CC := clang++
CFLAGS := -Wall -Werror -Wextra --std=c++98
DFLAGS := -g3
SRC := main.cpp sockets.cpp socketsIO.cpp
all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

debug: $(SRC)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $(NAME)

clean:
	/bin/rm -f $(OBJS)

fclean: clean
	/bin/rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re
