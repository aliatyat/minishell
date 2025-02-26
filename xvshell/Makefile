CC = cc

CFLAGS = -Wall -Wextra -Werror

INCLUDE = -Iinclude

NAME = minishell
#buildcmd.c  libft  Makefile  out  parsing.c  runcmd.c  tokens.c  xvshell.c  xvshell.h
SRCS =\
	src/xvshell.c\
	src/tokens.c\
	src/parsing.c\
	src/buildcmd.c\
	src/runcmd.c
	

OBJS = $(SRCS:.c=.o)

LIBS = -lreadline

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDE) $(LIBS) -o $(NAME)

%.o: %.c ./include/xvshell.h
	$(CC) $(CFLAGS) $(INCLUDE)  -c $< -o $@


clean :
	rm  -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
