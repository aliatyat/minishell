/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 20:24:55 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 22:09:09 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_herdoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	protect_new_ex(char *new_ex, int pipe_zero, int pipe_one)
{
	if (!new_ex)
	{
		free(new_ex);
		close(pipe_zero);
		close(pipe_one);
		return (-1);
	}
	return (1);
}

int	is_fully_quoted(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '\"' && str[len
				- 1] == '\"'))
		return (1);
	return (0);
}

int	check_exit(char *line, char *delimiter)
{
	if (!line || !ft_strcmp(line, delimiter))
	{
		free(line);
		return (0);
	}
	return (1);
}
