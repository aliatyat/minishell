/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:11:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 14:18:26 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_command *cmd, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]);
	free_env(shell->env);
	free_commands(shell->commands);
	exit(exit_code);
}

int	ft_export(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*equal_sign;
	int		status;

	i = 1;
	status = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(shell->env, cmd->out_fd);
		return (0);
	}
	while (cmd->args[i])
	{
		equal_sign = ft_strchr(cmd->args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			shell->env = update_env_var(shell->env, cmd->args[i], equal_sign
					+ 1);
			*equal_sign = '=';
			if (!shell->env)
				status = 1;
		}
		i++;
	}
	return (status);
}

int	ft_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!cmd->args[1])
		return (0);
	while (cmd->args[i])
	{
		shell->env = remove_env_var(shell->env, cmd->args[i]);
		if (!shell->env)
			status = 1;
		i++;
	}
	return (status);
}

void	ft_pwd(t_command *cmd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		ft_putendl_fd(cwd, cmd->out_fd);
	else
		ft_perror("pwd", 1);
	close(cmd->out_fd);
}

void	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '=')) // Only print variables with values
			ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		i++;
	}
}
