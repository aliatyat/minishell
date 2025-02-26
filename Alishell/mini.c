#include "pipex.h"

void	handle_redirection(char *cmd) 
{
	int fd = 0;
	char *file;

	if (ft_strstr(cmd, ">")) 
	{
		file = ft_strchr(cmd, '>') + 1;
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	} 
	else if (ft_strstr(cmd, ">>")) 
	{
		file = ft_strchr(cmd, '>') + 2;
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
}

int has_redirection(char *cmd)
{
    int i = 0;
    
    while (cmd[i])
    {
        if (cmd[i] == '>' || cmd[i] == '<')
        {
            if (cmd[i] == '>' && cmd[i + 1] == '>')
                return 1;
            return 1;
        }
        i++;
    }
    
    return 0;
}


int	is_builtin(char *cmd) 
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export"));
}

void	execute_builtin(char *cmd) 
{
	if (!ft_strcmp(cmd, "cd"))
		chdir(ft_strchr(cmd, ' ') + 1);
	else if (!ft_strcmp(cmd, "echo"))
		ft_putstr_fd(ft_strchr(cmd, ' ') + 1, 1);
	// Implement other built-ins...
}

char	**ft_split_pipes(char *input)
{
	char **cmds;
	int i;
	char *token;

	cmds = malloc(100 * sizeof(char *));
	if (!cmds)
		return (NULL);
	
	i = 0;
	token = ft_strtok(input, "|");
	while (token)
	{
		cmds[i] = ft_strdup(token);
		token = ft_strtok(NULL, "|");
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

void execute_external(char *cmd, char **envp) 
{
	char **cmds;
	char *path;

	cmds = ft_split(cmd, ' ');
	if (!cmds || !cmds[0])
		return;
	path = find_path(envp, cmds);
	if (!path)
		return;
	execve(path, cmds, envp);
	perror("execve error");
	exit(EXIT_FAILURE);
}



int minishell_pipex(char *input, char **envp)
{
    int i;
    int num_cmds;
    int **pipes;
    pid_t pid;
    char **cmds;

    // Split input into commands
    cmds = ft_split_pipes(input);
    if (!cmds)
        return (EXIT_FAILURE);

    // Count number of commands
    num_cmds = 0;
    while (cmds[num_cmds])
        num_cmds++;

    // Allocate pipes dynamically
    pipes = malloc((num_cmds - 1) * sizeof(int *));
    if (!pipes)
        return (EXIT_FAILURE);

    i = 0;
    while (i < num_cmds - 1)
    {
        pipes[i] = malloc(2 * sizeof(int));
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe error");
            return (EXIT_FAILURE);
        }
        i++;
    }

    // Loop through each command
    i = 0;
    while (cmds[i])
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            return (EXIT_FAILURE);
        }
        if (pid == 0) // Child Process
        {
            // Handle input and output redirection with pipes
            if (i > 0) // Not first command, get input from previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
            if (cmds[i + 1]) // Not last command, output to next pipe
                dup2(pipes[i][1], STDOUT_FILENO);

            // Handle Redirections (>, >>, <)
            if (has_redirection(cmds[i])) 
                handle_redirection(cmds[i]);

            // Close all pipes in child process
            int j = 0;
            while (j < num_cmds - 1)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
                j++;
            }

            // Check if built-in or external command
            if (is_builtin(cmds[i]))
                execute_builtin(cmds[i]);
            else
                execute_external(cmds[i], envp);

            exit(EXIT_SUCCESS);
        }
        i++;
    }

    // Parent Process: Close all pipes
    i = 0;
    while (i < num_cmds - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }

    // Wait for all child processes
    i = 0;
    while (i < num_cmds)
    {
        wait(NULL);
        i++;
    }

    return (0);
}
