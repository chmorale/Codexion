NAME 		= codexion

CC 			= cc
CFLAGS 		= -Wall -Wextra -Werror -Iincludes -pthread
ASANFLAGS	= -fsanitize=address,undefined,leak 
DBGFLAGS	= -g3 -O0 -fno-omit-frame-pointer -fno-sanitize-recover=all

SRCDIR		= coders

SRC 		= actions.c codexion.c dongle_access.c dongle_queue.c dongle_utils.c free_utils.c ft_atoi.c heap_utils.c init.c monitor.c parser.c simulation.c utils.c

OBJ = $(addprefix $(SRCDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

asan: fclean
	$(MAKE) CFLAGS="$(CFLAGS) $(ASANFLAGS) $(DBGFLAGS)" all
	
$(SRCDIR)/%.o: $(SRCDIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re asan
