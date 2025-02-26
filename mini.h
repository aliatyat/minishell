#ifndef MINI_H
# define MINI_H

#include "LIBFT/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include "LIBFT/libft.h"


char    *find_path(char **env, char **cmd);
int     open_files(int argc, char **argv, int *fd_in, int *fd_out);
void    launch_first_child(char *cmd, int fds[2], int pipe_fd[2], char **envp);
void    launch_second_child(char *cmd, int pipe_fd[2], int fd_out, char **envp);








#endif