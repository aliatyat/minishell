/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:12:34 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 14:45:07 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(char **cmd_args)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd_args[i])
	{
		if (ft_strcmp(cmd_args[i], ">") == 0 && cmd_args[i + 1])
		{
			fd = open(cmd_args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			cmd_args[i] = NULL;
			cmd_args[i + 1] = NULL;
		}
		else if (ft_strcmp(cmd_args[i], ">>") == 0 && cmd_args[i + 1])
		{
			fd = open(cmd_args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			cmd_args[i] = NULL;
			cmd_args[i + 1] = NULL;
		}
		else if (ft_strcmp(cmd_args[i], "<") == 0 && cmd_args[i + 1])
		{
			fd = open(cmd_args[i + 1], O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
			cmd_args[i] = NULL;
			cmd_args[i + 1] = NULL;
		}
		i++;
	}
}

int	has_redirection(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			if (cmd[i] == '>' && cmd[i + 1] == '>')
				return (1);
			return (1);
		}
		i++;
	}
	return (0);
}

int has_one_redirection(char c)
{
	return (c == '>' || c == '<');
}

int has_two_redirection(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if(str[0] == '<' && str[1] == '<')
		return (1);
	return (0);
}