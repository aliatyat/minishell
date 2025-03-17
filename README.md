The Functions we need for the project 

// Parsing & Execution

char	**ft_split_pipes(char *input);         // Split commands by '|'  ✅

char	**ft_split_spaces(char *input);        // Split by spaces while handling quotes

char	*find_executable(char *cmd, char **envp);  // Search for the binary in PATH

void	execute_external(char *cmd, char **envp);  // Execute external commands with execve  ✅

void	execute_builtin(char *cmd);           // Execute built-in commands like cd, echo   ✅


// Built-in Commands

int	is_builtin(char *cmd);                  // Check if command is built-in ✅

void	builtin_cd(char *path);               // Handle cd command

void	builtin_echo(char **args);            // Handle echo command ✅

void	builtin_export(char *var);            // Handle export VAR=value ✅

void	builtin_exit(char **args);            // Handle exit


// Redirections  ✅ ✅

void	handle_redirection(char *cmd);        // Handle '<', '>', '>>'    ✅

int	has_redirection(char *cmd);            // Check if command has redirection  ✅


// Pipes ✅✅

void	setup_pipes(int num_cmds, int pipes[num_cmds-1][2]);  // Create pipes ✅

void	close_pipes(int num_cmds, int pipes[num_cmds-1][2]); // Close pipes after use ✅


// Environment Variables

char	*get_env_value(char *name, char **envp); // Get value of an environment variable

void	set_env_value(char *name, char *value); // Set environment variable


// Signals

void	sig_handler(int sig);                 // Handle Ctrl+C, Ctrl+D, Ctrl+\ signals

void	setup_signals(void);                  // Setup all signal handlers


// Memory Management
void	free_2d_array(char **array);          // Free double pointer arrays

void	clean_exit(int status);               // Cleanup before exiting
