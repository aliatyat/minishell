/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:30 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/26 15:53:45 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_option(t_command *cmd, int *i)
{
	if (cmd->args[1] && !ft_strcmp(cmd->args[1], "-n"))
	{
		(*i)++;
		return (0);
	}
	return (1);
}

void	print_exit_status(t_shell *shell, int out_fd)
{
	(void)shell;
	printf("IN ????:\n");
	char	*exit_status1;

	exit_status1 = ft_itoa(exit_status);
	ft_putstr_fd(exit_status1, out_fd);
	//free(exit_status);
}

void	print_env_var(char *var_start, char **arg, t_shell *shell, int out_fd)
{
	char	*var_name;
	char	*value;

	while (**arg && (ft_isalnum(**arg) || **arg == '_'))
		(*arg)++;
	var_name = ft_substr(var_start, 0, *arg - var_start);
	value = get_env_value(shell->env, var_name);
	if (value)
		ft_putstr_fd(value, out_fd);
	free(var_name);
}

int	process_arg(char *arg, t_shell *shell, int out_fd)
{
	// int	in_single_quote;
	// int	in_double_quote;

	// in_single_quote = 0;
	// in_double_quote = 0;
	while (*arg)
	{
		// if (*arg == '\'' && !in_double_quote)
		// {
		// 	in_single_quote = !in_single_quote;
		// 	arg++;
		// 	continue ;
		// }
		// else if (*arg == '"' && !in_single_quote)
		// {
		// 	in_double_quote = !in_double_quote;
		// 	arg++;
		// 	continue ;
		//}
		if (*arg == '$' && *(arg + 1))
		{
			arg++;
			if (*arg == '?')
			{
				print_exit_status(shell, out_fd);
				arg++;
				continue ;
			}
			print_env_var(arg, &arg, shell, out_fd);
		}
		else
		{
			ft_putchar_fd(*arg, out_fd);
			arg++;
		}
	}
	return (0);
}

char	*ft_echo(t_command *cmd, t_shell *shell)
{
	
	int	i;
	int newline;
	(void)shell;
	i = 1;
	newline = handle_option(cmd, &i);
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		 //process_arg(cmd->args[i], shell, cmd->out_fd);
		// if (cmd->args[i + 1])
		// 	ft_putchar_fd(' ', cmd->out_fd);
		i++;
	}
	printf("\n");
	// if (newline)
	// 	ft_putchar_fd('\n', cmd->out_fd);
	return (NULL);
}
