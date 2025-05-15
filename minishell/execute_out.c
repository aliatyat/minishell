/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_out.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:22 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:24:45 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;

	(void)pid;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait(&status);
	if (WIFEXITED(status))
	{
		g_signal_status = -1;
		shell->exit_status = WEXITSTATUS(status);
		return (shell->exit_status);
	}
	if (WIFSIGNALED(status))
	{
		shell->exit_status = -1;
		g_signal_status = 128 + WTERMSIG(status);
		return (g_signal_status);
	}
	return (0);
}

int	prepare_command_arguments(t_command *cmd, t_shell *shell)
{
	int	i;
	int	f;

	i = 1;
	f = 0;
	if (ft_strchr(cmd->args[0], '$'))
		cmd->args[0] = expand_input(cmd->args[0], shell);
	else if (cmd->args[0][0] == '\"' || cmd->args[0][0] == '\'')
	{
		while (cmd->args[0][i] && cmd->args[0][i] != '\"'
			&& cmd->args[0][i] != '\'')
		{
			if (ft_isspace(cmd->args[0][i]))
			{
				f = 1;
				break ;
			}
			i++;
		}
		if (f == 0)
			cmd->args[0] = expand_input(cmd->args[0], shell);
	}
	return (0);
}

int	resolve_command_path(t_command *cmd, t_shell *shell, char **full_path)
{
	char	*temp;
	int		x;

	if (!cmd->args[0])
		return (0);
	temp = ft_strdup(cmd->args[0]);
	if (!temp)
		return (0);
	x = 0;
	while (temp[x] && temp[x] != ' ')
		x++;
	temp[x] = '\0';
	*full_path = find_path(shell->env, temp);
	free(temp);
	if (!*full_path)
	{
		ft_error(cmd->args[0], "command not found", 127);
		return (127);
	}
	return (0);
}

int	prepare_external_command(t_command *cmd, t_shell *shell, char **full_path)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	status = prepare_command_arguments(cmd, shell);
	if (status != 0)
		return (status);
	status = resolve_command_path(cmd, shell, full_path);
	return (status);
}

int	execute_external(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*full_path;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	status = prepare_external_command(cmd, shell, &full_path);
	if (status != 0)
		return (status);
	pid = fork();
	if (pid == -1)
	{
		free(full_path);
		return (1);
	}
	if (pid == 0)
		handle_child_process(cmd, shell, full_path);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		free(full_path);
		return (handle_parent_process(pid, shell));
	}
	return (0);
}
