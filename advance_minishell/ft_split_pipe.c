/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/17 17:18:24 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int	i;
	char	*token;
	char	*input_copy;

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
		char *trimmed;

		trimmed = ft_strtrim(token, " \t\n");
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
