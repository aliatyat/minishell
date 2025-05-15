/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:49 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:03:41 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_signal_status;

void	process_input(t_shell *shell, char *input)
{
	add_history(input);
	shell->input = input;
	shell->commands = parse_input(input, shell);
	if (shell->commands == NULL)
	{
		shell->exit_status = 2;
		return ;
	}
	if (shell->commands)
	{
		shell->exit_status = execute_command(shell->commands, shell);
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

static int	usage_minishell(char *argv, int argc)
{
	if (argv && argc > 1)
	{
		ft_putstr_fd("USAGE :./minishell no arg\n", 2);
		return (1);
	}
	return (0);
}

static int	handle_ctrl_d(t_shell *shell, char *input)
{
	if (!input)
	{
		handle_eof(shell);
		return (-1);
	}
	return (0);
}

static void	run_shell_loop(t_shell *shell, char **argv)
{
	char	*input;
	int		save_exit;
	int		x;

	save_exit = 0;
	while (1)
	{
		handle_signal();
		shell->argv_z = argv[0];
		shell->exit_status = save_exit;
		input = readline("minishell$ ");
		if (handle_ctrl_d(shell, input) == -1)
			break ;
		x = 0;
		while (input[x] && input[x] == ' ')
			x++;
		if (input[x] == '\0')
			continue ;
		process_input(shell, input);
		save_exit = shell->exit_status;
		close_all_pipes(shell->commands);
		free(input);
	}
}

int	check_lvl(t_shell *shell);

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		save_exit;

	save_exit = 0;
	if (usage_minishell(argv[1], argc) == 1)
		return (1);
	if (!init_shell(&shell, envp))
		return (EXIT_FAILURE);
	check_lvl(&shell);
	run_shell_loop(&shell, argv);
	free_env(shell.env);
	return (g_signal_status);
}
