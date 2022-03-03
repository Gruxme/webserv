NAME := webserv
CC := clang++
CFLAGS := -Wall -Werror -Wextra --std=c++98 
DFLAGS := -g3
SRC := main.cpp \
./src/config/ConfigClass.cpp ./src/config/ServerConfigClass.cpp ./src/config/LocationClass.cpp \
./src/request/Request.cpp \
./src/response/response.cpp ./src/response/mimeTypes.cpp \
./src/sockets/sockets.cpp ./src/sockets/socketsIO.cpp

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
