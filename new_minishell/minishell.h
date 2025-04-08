/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:12:22 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 15:33:58 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "LIBFT/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/* Signal numbers (standard POSIX values) */
# ifndef SIGINT
#  define SIGINT 2
# endif

# ifndef SIGQUIT
#  define SIGQUIT 3
# endif

/* Signal action macros */
# ifndef SIG_DFL
#  define SIG_DFL (void (*)(int))0
# endif

# ifndef SIG_IGN
#  define SIG_IGN (void (*)(int))1
# endif

typedef struct s_command
{
	char				**args;
	int					in_fd;
	int					out_fd;
	int					pipefd[2];
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				**env;
	int					exit_status;
	t_command			*commands;
}						t_shell;

// Environment functions
char					**copy_env(char **envp);
char					*get_env_value(char **env, char *name);
char					**update_env_var(char **env, char *name,
							const char *value);
void					free_env(char **env);
char					**remove_env_var(char **env, char *var);
void					print_sorted_env(char **env, int fd);
char					*find_path(char **env, char *cmd);
char					**get_paths_from_env(char **env);
void					close_all_pipes(t_command *commands);
char					**env_to_array(char **env);

// Command execution
int						execute_command(t_command *cmd, t_shell *shell);
int						execute_builtin(t_command *cmd, t_shell *shell);
int						execute_external(t_command *cmd, t_shell *shell);

// Builtins
int						ft_cd(t_command *cmd, t_shell *shell);
void					ft_echo(t_command *cmd, t_shell *shell);
void					ft_env(t_shell *shell);
void					ft_exit(t_command *cmd, t_shell *shell);
int						ft_export(t_command *cmd, t_shell *shell);
void					ft_pwd(t_command *cmd);
int						ft_unset(t_command *cmd, t_shell *shell);

// Parsing
t_command				*parse_input(char *input, t_shell *shell);
void					free_commands(t_command *cmd);

// Utils
void					free_2d_array(char **array);
char					*ft_strjoin3(const char *s1, const char *s2,
							const char *s3);
char					**ft_realloc_strarr(char **arr, size_t new_size);
int						ft_error(char *cmd, char *msg, int status);
int						ft_perror(char *cmd, int status);
void					free_split(char **array);
char					**ft_split_shell(char *str, char delim);
int						is_builtin(char *cmd);

// Redirection
void					handle_redirection(char **cmd_args);
int						has_redirection(char *cmd);
// int						has_two_redirection(char *str);
// int						has_one_redirection(char c);

char					**split_with_redirections(char *input);
void					handle_sigint(int sig);
char					**remove_null_args(char **args);
char					**ft_split_pipes(char *input);
int						execute_pipeline(t_command *cmd, t_shell *shell);
#endif