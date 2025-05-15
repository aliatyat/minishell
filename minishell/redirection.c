/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:06 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:31:26 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rebuild_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i] != NULL)
		{
			cmd->args[j++] = cmd->args[i];
		}
		else
		{
			free(cmd->args[i]);
		}
		i++;
	}
	cmd->args[j] = NULL;
}

int	redirection_tokens(char ***new_args, char **temp_split, int *j)
{
	*new_args = ft_realloc(*new_args, (*j) * sizeof(char *), ((*j) + 3)
			* sizeof(char *));
	if (!*new_args)
	{
		free_2d_array(temp_split);
		free_2d_array(*new_args);
		return (-1);
	}
	if (temp_split[0])
		(*new_args)[(*j)++] = temp_split[0];
	if (temp_split[1])
		(*new_args)[(*j)++] = temp_split[1];
	if (temp_split[2])
	{
		(*new_args)[(*j)++] = temp_split[2];
		temp_split[2] = NULL;
	}
	free(temp_split);
	return (0);
}

static int	split_redirection(t_command *cmd, char ***new_args, int *j, int i)
{
	char	**temp_split;

	if (ft_strchr(cmd->args[i], '>') || ft_strchr(cmd->args[i], '<'))
	{
		temp_split = split_with_redirections(cmd->args[i]);
		if (!temp_split)
			return (-1);
		if (redirection_tokens(new_args, temp_split, j) == -1)
			return (-1);
	}
	else
	{
		*new_args = ft_realloc(*new_args, (*j) * sizeof(char *),
				((*j) + 1) * sizeof(char *));
		if (!*new_args)
			return (-1);
		(*new_args)[(*j)++] = ft_strdup(cmd->args[i]);
		if (!(*new_args)[(*j) - 1])
		{
			free_2d_array(*new_args);
			return (-1);
		}
	}
	return (0);
}

static int	extract_redirections(t_command *cmd, char ***new_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	*new_args = NULL;
	while (cmd->args[i])
	{
		if (split_redirection(cmd, new_args, &j, i) == -1)
			return (-1);
		i++;
	}
	*new_args = ft_realloc(*new_args, j * sizeof(char *),
			(j + 1) * sizeof(char *));
	if (!*new_args)
		return (-1);
	(*new_args)[j] = NULL;
	return (0);
}

int	handle_redirection1(t_command *cmd, t_shell *shell)
{
	char	**new_args;

	if (extract_redirections(cmd, &new_args) == -1)
		return (-1);
	if (cmd->args)
		free_2d_array(cmd->args);
	cmd->args = new_args;
	if (process_redirections(cmd, shell) == -1)
		return (-1);
	rebuild_args(cmd);
	return (0);
}
