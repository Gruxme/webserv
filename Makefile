NAME = webserv
CC = clang++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address
SRC = main.cpp \
./src/config_parser/ConfigFile.cpp \
./src/config_parser/ServerConfig.cpp \
./src/config_parser/LocationClass.cpp \
./src/request_parser/Request.cpp \

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re