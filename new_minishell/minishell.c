#include "minishell.h"

// Free a NULL-terminated array of strings
void	free_2d_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") ||
			!ft_strcmp(cmd, "exit"));
}

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int		i;
	char	*token;

	cmds = malloc(100 * sizeof(char *));
	if (!cmds)
		return (NULL);
	i = 0;
	token = ft_strtok(input, "|");
	while (token)
	{
		cmds[i] = ft_strdup(token);
		if (!cmds[i]) // Handle strdup failure
		{
			free_2d_array(cmds);
			return (NULL);
		}
		token = ft_strtok(NULL, "|");
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

void	execute_external(char *cmd, char **envp)
{
	char	**cmds;
	char	*path;

	cmds = ft_split(cmd, ' ');
	if (!cmds || !cmds[0])
		return ;
	path = find_path(envp, cmds);
	if (!path)
	{
		free_2d_array(cmds);
		perror("command not found");
		exit(EXIT_FAILURE);
	}
	execve(path, cmds, envp);
	perror("execve error");
	exit(EXIT_FAILURE);
}

// Function 1: Count commands in the array
// int count_commands(char **cmds)
// {
//     int count = 0;
//     while (cmds[count])
//         count++;
//     return (count);
// }

// // Function 2: Initialize pipes for commands
// int **init_pipes(int num_pipes)
// {
//     int **pipes = malloc(num_pipes * sizeof(int *));
//     if (!pipes)
//         return (NULL);

//     int i = 0;
//     while (i < num_pipes)
//     {
//         pipes[i] = malloc(2 * sizeof(int));
//         if (!pipes[i] || pipe(pipes[i]) == -1)
//         {
//             perror("pipe error");
//             return (NULL);
//         }
//         i++;
//     }
//     return (pipes);
// }

// // Function 3: Setup pipe redirections for child process
// void setup_child_pipes(int i, int num_cmds, int **pipes)
// {
//     // Redirect input from previous pipe (if not first command)
//     if (i > 0)
//         dup2(pipes[i - 1][0], STDIN_FILENO);

//     // Redirect output to next pipe (if not last command)
//     if (i < num_cmds - 1)
//         dup2(pipes[i][1], STDOUT_FILENO);

//     // Close all pipes in child
//     int j = 0;
//     while (j < num_cmds - 1)
//     {
//         close(pipes[j][0]);
//         close(pipes[j][1]);
//         j++;
//     }
// }

// // Function 4: Execute single command in child process
// void execute_command(char **cmd, char ***envp)
// {
//     int i = 0;
//     int **pipes;
//     if (i > 0)
//         dup2(pipes[i - 1][0], STDIN_FILENO);

//     // Redirect output to next pipe (if not last command)
//     if (i < num_cmds - 1)
//         dup2(pipes[i][1], STDOUT_FILENO);

//     // Close all pipes in child
//     int j = 0;
//     while (j < num_cmds - 1)
//     {
//         close(pipes[j][0]);
//         close(pipes[j][1]);
//         j++;
//     }
//     char **cmd_args = ft_split_pipes(cmd[i]);
//     if (!cmd_args)
//         exit(EXIT_FAILURE);
//     while(cmd[i])
//     {

//     // Handle redirections
//     if (has_redirection(cmd[i]))
//         cmd_args = ft_split(cmd[i], ' ');
//     handle_redirection1(cmd_args);

//     // Execute command
//     if (is_builtin(cmd_args[0]))
//         execute_builtin(cmd_args, *envp);
//     else
//         execute_external(cmd_args[0], *envp);
//     }
//     i++;

//     free_2d_array(cmd_args);
//     exit(EXIT_SUCCESS);
// }

// // Function 5: Cleanup resources in parent process
// void parent_cleanup(int num_cmds, int **pipes, char **cmds)
// {
//     // Close all pipes in parent
//     int i = 0;
//     while (i < num_cmds - 1)
//     {
//         close(pipes[i][0]);
//         close(pipes[i][1]);
//         free(pipes[i]);
//         i++;
//     }
//     free(pipes);
//     free_2d_array(cmds);
// }

// // Main function (now simplified)
