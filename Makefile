NAME := webserv
CC := clang++
CFLAGS := -Wall -Werror -Wextra --std=c++98
DFLAGS := -fsanitize=address -g3
SRC := main.cpp
HPP := sockets.hpp socketsIO.hpp
all: $(NAME)

$(NAME): $(SRC) $(HPP)
	$(CC) $(CFLAGS) $< -o $@

debug: $(SRC) $(HPP)
	$(CC) $(CFLAGS) $(DFLAGS) $< -o $(NAME)

clean:
	/bin/rm -f $(OBJS)

fclean: clean
	/bin/rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re