NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -pthread

UTILS_DIR = utils/

SRC = actions.c clean_and_exit.c codexion.c dongle_access.c ft_atoi.c init.c parser.c simulation.c utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
