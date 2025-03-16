#include "pipex.h"


char *expand_env_var(char *arg, char **envp)
{
    if (arg[0] == '$')
    {
        char *var_name = arg + 1; // Skip the '$' character
        int i = 0;
        while (envp[i])
        {
            if (!ft_strncmp(envp[i], var_name, ft_strlen(var_name)) && envp[i][ft_strlen(var_name)] == '=')
            {
                return (envp[i] + ft_strlen(var_name) + 1); // Return the value of the environment variable
            }
            i++;
        }
    }
    return arg; // Return the original argument if no environment variable is found
}

void	ft_echo(char **args, char **envp)
{
	int i = 1;
	int newline = 1;

	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		char *expanded_arg = expand_env_var(args[i], envp);
		ft_putstr_fd(expanded_arg, STDOUT_FILENO);
		//ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}


void	ft_cd(char **args)
{
	if (!args[1]) // No argument, go to HOME
		chdir(getenv("HOME"));
	else if (chdir(args[1]) != 0)
		perror("cd");
}

void	ft_exit(char **args)
{
	int exit_code = 0;

	if (args[1])
		exit_code = ft_atoi(args[1]);
	exit(exit_code);
}

void	ft_env(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
    }
}

char	**add_env_variable(char **envp, char *new_var)
{
	int		i;
	int		env_size;
	char	**new_envp;

	// Count current env variables
	env_size = 0;
	while (envp[env_size])
		env_size++;

	// Check if variable already exists (replace its value)
	for (i = 0; i < env_size; i++)
	{
		if (!ft_strncmp(envp[i], new_var, ft_strchr(new_var, '=') - new_var))
		{
			free(envp[i]);
			envp[i] = ft_strdup(new_var);
			return (envp);
		}
	}

	// Allocate new env with extra space
	new_envp = malloc((env_size + 2) * sizeof(char *));
	if (!new_envp)
		return (NULL);

	// Copy old env
	for (i = 0; i < env_size; i++)
		new_envp[i] = envp[i];
	new_envp[i] = ft_strdup(new_var); // Add new variable
	new_envp[i + 1] = NULL; // Terminate the array

	free(envp); // Free old envp (optional, depends on memory management)
	return (new_envp);
}


char	**remove_env_variable(char **envp, char *var)
{
	int		i;
	int		j;
	int		env_size;
	char	**new_envp;

	// Count env variables
	env_size = 0;
	while (envp[env_size])
		env_size++;

	// Allocate new env without the removed var
	new_envp = malloc(env_size * sizeof(char *));
	if (!new_envp)
		return (NULL);

	j = 0;
	for (i = 0; i < env_size; i++)
	{
		// Skip the variable to remove
		if (!ft_strncmp(envp[i], var, ft_strlen(var)) && envp[i][ft_strlen(var)] == '=')
			free(envp[i]);
		else
			new_envp[j++] = envp[i];
	}
	new_envp[j] = NULL;

	free(envp); // Free old envp (optional)
	return (new_envp);
}



void	ft_unset(char **args, char **envp)
{
	if (args[1])
		envp = remove_env_variable(envp, args[1]);
}


void	ft_export(char **args, char **envp)
{
	if (!args[1]) // No argument, print all environment variables
		ft_env(envp);
	else
	{
		// Add new variable to envp
		envp = add_env_variable(envp, args[1]);
	}
}

void	ft_pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		ft_putendl_fd(cwd, STDOUT_FILENO);
	else
		perror("pwd");
}


int	execute_builtin(char **args, char **envp)
{
	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args, envp);
	else if (!ft_strcmp(args[0], "cd"))
		ft_cd(args);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, envp);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(args, envp);
	// else if (!ft_strcmp(args[0], "env"))
	// 	ft_env(*envp);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args);
	else
		return (0);
	return (1);
}




// int main(int argc, char **argv)
// {
//     if (argc < 2)
//     {
//         ft_putstr_fd("Usage: ./pipex \"cmd1 | cmd2 | ...\" [file1] [file2]\n", STDERR_FILENO);
//         return (EXIT_FAILURE);
//     }
//     if (!ft_strcmp(argv[1], "echo"))
//         builtin_echo(argv);
//     return (EXIT_SUCCESS);
// }