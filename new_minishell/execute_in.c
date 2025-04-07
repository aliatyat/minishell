/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:11:56 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 14:32:03 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int	i;

	char *builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
	i = 0;
	if (!cmd)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	handle_common_builtins(char *command, t_command *cmd,
		t_shell *shell)
{
	if (ft_strcmp(command, "echo") == 0)
	{
		ft_echo(cmd, shell);
		return (shell->exit_status = 0);
	}
	else if (ft_strcmp(command, "pwd") == 0)
	{
		ft_pwd(cmd);
		return (1);
	}
	else if (ft_strcmp(command, "env") == 0)
	{
		ft_env(shell);
		return (1);
	}
	return (0);
}

static int	handle_special_builtins(char *command, t_command *cmd,
		t_shell *shell)
{
	if (ft_strcmp(command, "cd") == 0)
	{
		shell->exit_status = ft_cd(cmd, shell);
		return (1);
	}
	else if (ft_strcmp(command, "export") == 0)
	{
		shell->exit_status = ft_export(cmd, shell);
		return (1);
	}
	else if (ft_strcmp(command, "unset") == 0)
	{
		shell->exit_status = ft_unset(cmd, shell);
		return (1);
	}
	else if (ft_strcmp(command, "exit") == 0)
	{
		ft_exit(cmd, shell);
		return (1);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	command = cmd->args[0];
	if (handle_common_builtins(command, cmd, shell))
		return (1);
	return (handle_special_builtins(command, cmd, shell));
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = 1; // Set error status
		return (1);
	}
	// Check if we have a pipeline or single command
	if (cmd->next)
	{
		return (execute_pipeline(cmd, shell));
	}
	// Handle single command
	if (is_builtin(cmd->args[0]))
	{
		return (execute_builtin(cmd, shell));
	}
	else
	{
		return (execute_external(cmd, shell));
	}
}
