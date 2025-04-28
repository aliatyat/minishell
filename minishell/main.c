/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:49 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/28 18:21:02 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//t_shell	*g_shell = NULL;
int exit_status;
// Signal handler function
void	handle_sigint(int sig)
{
	//t_shell	*g_shell = NULL;
	(void)sig;
	// if (g_shell)
	// {
		// Print new prompt on new line
		write(STDOUT_FILENO, "\n", 1);
		if (rl_on_new_line() == -1)
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		// Set exit status to 130 (like bash)
		exit_status = 128 + SIGINT;
	//}
}
void increment_shell_level(t_shell *shell)
{ 
	int shlvl = 0;
	char *shlvl_str = get_env_value(shell->env, "SHLVL");
	if (shlvl_str)
   	 	shlvl = ft_atoi(shlvl_str) + 1; // Increment SHLVL
	char *new_shlvl = ft_itoa(shlvl); // Convert back to string
	shell->env = update_env_var(shell->env, "SHLVL", new_shlvl); // Update SHLVL in env
free(new_shlvl); // Free allocated memory
}

int	init_shell(t_shell *shell, char **envp)
{
	
	
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env = copy_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	//increment_shell_level(shell);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: environment copy failed", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	set_initial_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		shell->env = update_env_var(shell->env, "PWD", cwd);
	return 1;
}

void	process_input(t_shell *shell, char *input)
{
	add_history(input);
	shell->input = input;
	shell->commands = parse_input(input, shell);
	if (shell->commands)
	{
		int g = 0;
		while (shell->commands->args[g])
		{
			printf("WWWWWWWWWWWW: %s\n", shell->commands->args[g]);
			g++;
		}
		
		// Execute the command chain
		exit_status = execute_command(shell->commands, shell);
		// Clean up commands after execution
		free_commands(shell->commands);
		shell->commands = NULL; // Important: set to NULL after freeing
	}
}

void	handle_eof(t_shell *shell)
{
	if (shell->exit_status == 0)
		ft_putendl_fd("exit", STDOUT_FILENO);
}

void	cleanup_shell(t_shell *shell)
{
	//g_shell = NULL;
	free_env(shell->env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	if (!init_shell(&shell, envp))
		return (EXIT_FAILURE);
	increment_shell_level(&shell);
	// if (!set_initial_pwd(&shell))
	// 	return 0;
	while (1)
	{
		signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
		input = readline("minishell$ ");
		
		if (!input)
		{
			handle_eof(&shell);
			break ;
		}
		// if (*input == '\0')
		// {
		// 	free(input);
		// 	continue ;
		// }
		process_input(&shell, input);
		free(input);
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}
