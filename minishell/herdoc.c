/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 18:39:00 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_loop(char *delimiter, char *new_ex, t_shell *shell,
		int *pipe_fd)
{
	char	*line;
	int		quoted;

	quoted = is_fully_quoted(delimiter) || is_fully_quoted(new_ex);
	while (1)
	{
		handle_signal_herdoc();
		line = readline("> ");
		if (check_exit(line, delimiter) == 0)
			break ;
		if (!quoted)
		{
			line = expand_here(line, shell);
			if (!line)
				return (-1);
		}
		write_into_pipe(pipe_fd[1], line);
	}
	return (0);
}

static void	free_pointer(char *new_ex, char *trimmed)
{
	if (new_ex)
		free(new_ex);
	if (trimmed)
		free(trimmed);
}

static int	protect_trimmed(char *trimmed, char *new_ex)
{
	if (trimmed == NULL)
	{
		free(new_ex);
		return (-1);
	}
	return (0);
}

static int	handle_while_herdoc(char *delimiter,
								t_shell *shell,
								int *pipe_fd)
{
	char	*new_ex;
	char	*trimmed;
	int		status;

	trimmed = NULL;
	new_ex = ft_strdup(delimiter);
	if (!new_ex)
		return (-1);
	if (protect_new_ex(new_ex, pipe_fd[0], pipe_fd[1]) == -1)
		return (-1);
	if (delimiter[0] == '\"' || delimiter[0] == '\'')
	{
		if (delimiter[0] == '\'')
			trimmed = ft_strtrim(delimiter, "\'");
		else if (delimiter[0] == '\"')
			trimmed = ft_strtrim(delimiter, "\"");
		if (protect_trimmed(trimmed, new_ex) == -1)
			return (-1);
		delimiter = trimmed;
	}
	status = heredoc_loop(delimiter, new_ex, shell, pipe_fd);
	free_pointer(new_ex, trimmed);
	return (status);
}

int	handle_heredoc(t_command *cmd, char *delimiter, t_shell *shell)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	if (handle_while_herdoc(delimiter, shell, pipe_fd) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	cmd->in_fd = pipe_fd[0];
	if (g_signal_status == 130)
		return (130);
	return (0);
}
