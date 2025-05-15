/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:23:05 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_pipes(const char *s)
{
	int		count;
	char	quote;

	count = 1;
	quote = 0;
	while (*s)
	{
		if ((*s == '\'' || *s == '"') && !quote)
			quote = *s;
		else if (*s == quote)
			quote = 0;
		else if (*s == '|' && !quote)
			count++;
		s++;
	}
	return (count);
}

static char	*extract_segment(const char *s, int *i)
{
	int		index[3];
	char	*substr;
	char	*trimmed;

	ft_memset(index, 0, sizeof(index));
	index[0] = *i;
	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '"') && !index[2])
			index[2] = s[*i];
		else if (s[*i] == index[2])
			index[2] = 0;
		else if (s[*i] == '|' && !index[2])
			break ;
		(*i)++;
	}
	index[1] = *i - index[0];
	while (s[*i] == '|')
		(*i)++;
	substr = ft_substr(s, index[0], index[1]);
	if (!substr)
		return (NULL);
	trimmed = ft_strtrim(substr, " \t\n");
	free(substr);
	return (trimmed);
}

static int	protect_cmds(char **cmds, char *seg, int *index)
{
	if (!seg)
	{
		free_2d_array(cmds);
		return (-1);
	}
	(*index)++;
	return (0);
}

static int	check_qoute(char **cmds)
{
	int	y;

	y = 0;
	while (cmds[y])
	{
		if (has_unclosed_quotes(cmds[y]) != 0)
		{
			printf("Unclosed quotes in: %s\n", cmds[y]);
			return (-1);
		}
		y++;
	}
	return (0);
}

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int		index[3];

	ft_memset(index, 0, sizeof(index));
	if (!input)
		return (NULL);
	index[2] = count_pipes(input);
	cmds = malloc(sizeof(char *) * (index[2] + 1));
	if (!cmds)
		return (NULL);
	while (index[0] < index[2])
	{
		cmds[index[0]] = extract_segment(input, &index[1]);
		if (protect_cmds(cmds, cmds[index[0]], &index[0]) == -1)
			return (NULL);
	}
	cmds[index[0]] = NULL;
	prioritize_heredocs(cmds, index[0]);
	if (check_qoute(cmds) == -1)
		return (NULL);
	return (cmds);
}
