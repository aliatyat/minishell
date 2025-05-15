/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 20:35:20 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/11 20:39:44 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes_here(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
	{
		*in_squote = !(*in_squote);
		return (1);
	}
	if (c == '"' && !(*in_squote))
	{
		*in_dquote = !(*in_dquote);
		return (1);
	}
	return (0);
}

void	write_into_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}
