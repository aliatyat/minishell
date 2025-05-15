/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:54:25 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 19:55:01 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_out_redir(t_command *cmd, int *i)
{
	int	x;

	x = 0;
	x = dup(cmd->out_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	cmd->out_fd = open(cmd->args[*i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
	{
		ft_perror("minishell", 1);
		close(x);
		return (-1);
	}
	free(cmd->args[*i]);
	free(cmd->args[*i + 1]);
	cmd->args[*i] = NULL;
	cmd->args[*i + 1] = NULL;
	*i += 2;
	close(x);
	return (0);
}

int	handle_append_redir(t_command *cmd, int *i)
{
	int	x;

	x = 0;
	x = dup(cmd->out_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	cmd->out_fd = open(cmd->args[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->out_fd == -1)
	{
		ft_perror("minishell", 1);
		return (-1);
	}
	free(cmd->args[*i]);
	free(cmd->args[*i + 1]);
	cmd->args[*i] = NULL;
	cmd->args[*i + 1] = NULL;
	*i += 2;
	close(x);
	return (0);
}

int	handle_in_redir(t_command *cmd, int *i)
{
	int	x;

	x = 0;
	x = dup(cmd->in_fd);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	cmd->in_fd = open(cmd->args[*i + 1], O_RDONLY);
	if (cmd->in_fd == -1)
	{
		ft_perror(cmd->args[*i + 1], 1);
		close(x);
		return (-1);
	}
	free(cmd->args[*i]);
	free(cmd->args[*i + 1]);
	cmd->args[*i] = NULL;
	cmd->args[*i + 1] = NULL;
	*i += 2;
	dup2(cmd->in_fd, STDIN_FILENO);
	close(x);
	return (0);
}

int	handle_heredoc_redir(t_command *cmd, int *i, t_shell *shell)
{
	if (handle_heredoc(cmd, cmd->args[*i + 1], shell) == -1)
		return (-1);
	if (cmd->args[*i + 2] == NULL || ft_strcmp(cmd->args[*i + 2], "<<") != 0)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
	}
	free(cmd->args[*i]);
	free(cmd->args[*i + 1]);
	cmd->args[*i] = NULL;
	cmd->args[*i + 1] = NULL;
	*i += 2;
	return (0);
}

int	has_redirection(char *cmd)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '\"'))
		{
			if (quote == 0)
				quote = cmd[i];
			else if (quote == cmd[i])
				quote = 0;
		}
		else if ((cmd[i] == '>' || cmd[i] == '<') && quote == 0)
		{
			if (cmd[i] == '>' && cmd[i + 1] == '>')
				return (1);
			return (1);
		}
		i++;
	}
	return (0);
}
