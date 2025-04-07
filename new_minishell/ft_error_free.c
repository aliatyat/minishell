/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:12:10 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 15:59:41 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error(char *cmd, char *msg, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	return (status);
}

int	ft_perror(char *cmd, int status)
{
	perror("minishell");
	if (cmd)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
	}
	return (status);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_split(tmp->args);
		if (tmp->in_fd != STDIN_FILENO)
			close(tmp->in_fd);
		if (tmp->out_fd != STDOUT_FILENO)
			close(tmp->out_fd);
		free(tmp);
	}
}

void	free_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int		i;
	char	*token;
	char	*input_copy;
	char	*trimmed;

	i = 0;
	if (!input)
		return (NULL);
	input_copy = ft_strdup(input);
	if (!input_copy)
		return (NULL);
	cmds = malloc(100 * sizeof(char *));
	if (!cmds)
	{
		free(input_copy);
		return (NULL);
	}
	token = ft_strtok(input_copy, "|");
	while (token)
	{
        printf("in\n");
		trimmed = ft_strtrim(token, " \t\n>");
		if (!trimmed)
		{
			free_2d_array(cmds);
			free(input_copy);
			return (NULL);
		}
		cmds[i] = trimmed;
		token = ft_strtok(NULL, "|");
		i++;
		if (i >= 99) // Prevent buffer overflow
			break ;
	}
	cmds[i] = NULL;
	free(input_copy);
	return (cmds);
}

// Free a NULL-terminated array of strings
void	free_2d_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
