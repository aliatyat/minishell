#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include "LIBFT/libft.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/* Signal numbers (standard POSIX values) */
#ifndef SIGINT
# define SIGINT 2
#endif

#ifndef SIGQUIT
# define SIGQUIT 3
#endif

/* Signal action macros */
#ifndef SIG_DFL
# define SIG_DFL (void (*)(int))0
#endif

#ifndef SIG_IGN
# define SIG_IGN (void (*)(int))1
#endif

typedef struct s_command 
{
    char        **args;
    int         in_fd;
    int         out_fd;
    struct s_command *next;
} t_command;

typedef struct s_shell 
{
    //char *g_shell;
    char        **env;
    int         exit_status;
    t_command   *commands;
} t_shell;

 //t_shell *g_shell; //= NULL;

// Environment functions
char    **copy_env(char **envp);
char    *get_env_value(char **env,  char *name);
char    **update_env_var(char **env,  char *name, const char *value);
void    free_env(char **env);
char    **remove_env_var(char **env,  char *var);
void    print_sorted_env(char **env, int fd);
char    *find_path(char **env, char *cmd);
char    **get_paths_from_env(char **env);
void    close_all_pipes(t_command *commands);
char    **env_to_array(char **env);

// Command execution
int     execute_command(t_command *cmd, t_shell *shell);
int     execute_builtin(t_command *cmd, t_shell *shell);
int    execute_external(t_command *cmd, t_shell *shell);

// Builtins
int     ft_cd(t_command *cmd, t_shell *shell);
char    *ft_echo(t_command *cmd, t_shell *shell);
void    ft_env(t_shell *shell);
void    ft_exit(t_command *cmd, t_shell *shell);
int    ft_export(t_command *cmd, t_shell *shell);
void    ft_pwd(t_command *cmd);
int   ft_unset(t_command *cmd, t_shell *shell);

// Parsing
t_command *parse_input(char *input, t_shell *shell);
void    free_commands(t_command *cmd);

// Utils
void    free_2d_array(char **array);
char    *ft_strjoin3(const char *s1, const char *s2, const char *s3);
char    **ft_realloc_strarr(char **arr, size_t new_size);
int     ft_error( char *cmd,  char *msg, int status);
int     ft_perror( char *cmd, int status);
void free_split(char **array);
char	**ft_split_shell(const char *str, char delim);
int is_builtin(char *cmd);

// Redirection
int handle_redirection1(t_command *cmd);
int has_redirection(char *cmd);
int handle_heredoc(t_command *cmd, char *delimiter);

void handle_sigint(int sig);

char **ft_split_pipes(char *input);
int execute_pipeline(t_command *cmd, t_shell *shell);
char	**remove_null_args(char **args);
char	**split_with_redirections(char *input);
int process_echo_arg(char *arg, t_shell *shell, int out_fd);
void print_env_var(char *var_start, char **arg, t_shell *shell, int out_fd);
#endif