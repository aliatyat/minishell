/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:54:58 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:06:12 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redir1(t_command *cmd, int *i)
{
	if (ft_strcmp(cmd->args[*i], ">") == 0 && cmd->args[*i + 1])
	{
		if (handle_out_redir(cmd, i) == -1)
			return (-1);
	}
	else if (ft_strcmp(cmd->args[*i], ">>") == 0 && cmd->args[*i + 1])
	{
		if (handle_append_redir(cmd, i) == -1)
			return (-1);
	}
	else
	{
		return (0);
	}
	return (1);
}

int	process_redir2(t_command *cmd, int *i, t_shell *shell)
{
	if (ft_strcmp(cmd->args[*i], "<") == 0 && cmd->args[*i + 1])
	{
		if (handle_in_redir(cmd, i) == -1)
			return (-1);
	}
	else if (ft_strcmp(cmd->args[*i], "<<") == 0 && cmd->args[*i + 1])
	{
		if (handle_heredoc_redir(cmd, i, shell) == -1)
			return (-1);
	}
	else
	{
		return (0);
	}
	return (1);
}

int	process_redirections(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if ((ft_strcmp(cmd->args[i], ">") == 0 && cmd->args[i + 1])
			|| (ft_strcmp(cmd->args[i], ">>") == 0 && cmd->args[i + 1]))
		{
			if (process_redir1(cmd, &i) == -1)
				return (-1);
		}
		else if ((ft_strcmp(cmd->args[i], "<") == 0 && cmd->args[i + 1])
			|| (ft_strcmp(cmd->args[i], "<<") == 0 && cmd->args[i + 1]))
		{
			if (process_redir2(cmd, &i, shell) == -1)
				return (-1);
		}
		else
		{
			i++;
		}
	}
	return (0);
}
