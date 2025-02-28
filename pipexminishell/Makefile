NAME = pipex
LIBFT = ./LIBFT/libft.a
SRC = pipex.c  pipex_u.c free_split.c mini.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -g
#prerequisite
all: $(NAME)

$(NAME): $(OBJ)
	make -C ./LIBFT
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
clean:
	rm -f $(OBJ)
	make clean -C ./LIBFT

fclean: clean
	rm -f $(NAME)
	make fclean -C ./LIBFT

re: fclean all

.PHONY: fclean clean all re
