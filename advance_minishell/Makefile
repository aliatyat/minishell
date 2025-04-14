NAME = minishell
LIBFT = ./LIBFT/libft.a
SRC = execute_in.c execute_out.c built_in.c ft_echo.c ft_cd.c main.c utils.c ft_error_free.c redirection.c env1.c env2.c pipe.c test.c herdoc.c
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

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
