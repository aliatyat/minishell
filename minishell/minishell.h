/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:59 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:25:30 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "LIBFT/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_command
{
	char				**args;
	int					in_fd;
	int					out_fd;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				**env;
	int					exit_status;
	int					last_fork;
	char				*argv_z;
	t_command			*commands;
	char				*input;
}						t_shell;

typedef struct s_pipe_var
{
	t_command			*current;
	t_command			*next;
	int					f;
}						t_pipe_var;

extern int				g_signal_status;
// Environment functions
char					**copy_env(char **envp);
char					*get_env_value(char **env, char *name);
char					**update_env_var(char **env, char *name, char *value);
void					free_env(char **env);
char					**remove_env_var(char **env, char *var);
void					print_sorted_env(char **env, int fd);
char					*find_path(char **env, char *cmd);
char					**get_paths_from_env(char **env);
void					close_all_pipes(t_command *commands);
char					**env_to_array(char **env);
int						count_env_vars(char **env);
char					**create_filtered_env(char **env, char *var,
							size_t var_len, int count);
char					*search_path_in_env(char **paths, char *cmd);

// Command execution
int						execute_command(t_command *cmd, t_shell *shell);
int						execute_builtin(t_command *cmd, t_shell *shell);
int						execute_external(t_command *cmd, t_shell *shell);

// Builtins
int						ft_cd(t_command *cmd, t_shell *shell);
char					*ft_echo(t_command *cmd, t_shell *shell);
int						ft_env(t_shell *shell);
int						ft_exit(t_command *cmd, t_shell *shell);
int						ft_export(t_command *cmd, t_shell *shell);
int						ft_pwd(t_command *cmd);
int						ft_unset(t_command *cmd, t_shell *shell);
int						is_builtin(char *cmd);
int						export_entry(char *arg, t_shell *shell);
int						print_invalid(char *arg, int status);

// Parsing
t_command				*parse_input(char *input, t_shell *shell);
void					free_commands(t_command *cmd);
void					expand_dollar_in(char **ptr, char *buffer,
							size_t *i, t_shell *shell);
int						handle_quotes_in(char c, int *in_squote,
							int *in_dquote);

// Utils
void					free_2d_array(char **array);
char					*ft_strjoin3(char *s1, char *s2,
							char *s3);
char					*ft_strjoin4(char *s1, char *s2, char *s3);
char					*ft_strndup(char *s, size_t n);
char					**ft_realloc_strarr(char **arr, size_t new_size);
int						ft_error(char *cmd, char *msg, int status);
int						ft_perror(char *cmd, int status);
void					free_split(char **array);
char					**ft_split_shell(char *str, char delim);
void					free_3d_array(char ***array);

// Redirection
int						handle_redirection1(t_command *cmd, t_shell *shell);
int						has_redirection(char *cmd);
int						process_redirections(t_command *cmd, t_shell *shell);
int						handle_out_redir(t_command *cmd, int *i);
int						handle_append_redir(t_command *cmd, int *i);
int						handle_in_redir(t_command *cmd, int *i);
int						handle_heredoc_redir(t_command *cmd, int *i,
							t_shell *shell);
void					handle_sigint(int sig);
char					*handle_redirection_token(char **ptr, int length);
char					*handle_quoted_string(char **ptr);
char					*skip_whitespace(char *ptr);

// Heredoc
int						handle_heredoc(t_command *cmd, char *delimiter,
							t_shell *shell);
int						is_fully_quoted(char *str);
int						check_exit(char *line, char *delimiter);
char					*expand_here(char *input, t_shell *shell);
int						protect_new_ex(char *new_ex, int pipe_zero,
							int pipe_one);
void					handle_signal_herdoc(void);
void					write_into_pipe(int fd, char *line);
int						handle_quotes_here(char c, int *in_squote,
							int *in_dquote);
void					prioritize_heredocs(char **cmds, int count);
// Signal
void					handle_signal(void);
void					handle_eof(t_shell *shell);
// Pipeline
void					parent_cleanup(int *prev_pipe_in, t_command *cmd);
int						start_pipe(t_command *current, t_command *next,
							int *pipefd);
void					execute_child(t_command *cmd, int prev_pipe_in,
							t_shell *shell);

char					**ft_split_pipes(char *input);
int						execute_pipeline(t_command *cmd, t_shell *shell);
char					**remove_null_args(char **args);
char					**split_with_redirections(char *input);

// Expand
char					*expand_input(char *input, t_shell *shell);
void					expand_loop(char *input, char *buffer, t_shell *shell);
int						has_unclosed_quotes(char *str);

// Normaliaze
char					**gather_back(char **tokens, char *cmd, char **args,
							char **redir);
void					fill_args_and_redir(char **tokens, char **args,
							char **redir, char **cmd);
char					*join_red(char *s1, char *s2, int *i);

int						handle_child_process(t_command *cmd, t_shell *shell,
							char *full_path);
int						init_shell(t_shell *shell, char **envp);
char					**normalize_segment(char **tokens, int start,
							int end);
int						is_invalid_syntax(char *input);
void					wait_for_children(t_shell *shell);
int						count_tokens2(char **tokens);
int						is_redirection(char *s);
int						is_redirection_token(char *token);

#endif