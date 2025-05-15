/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:17 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:26:10 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(const char **str, int *in_quotes, char *quote_char)
{
	if (!*in_quotes && (**str == '\'' || **str == '"'))
	{
		*quote_char = **str;
		*in_quotes = 1;
		(*str)++;
		return (1);
	}
	else if (*in_quotes && **str == *quote_char)
	{
		*in_quotes = 0;
		(*str)++;
		return (1);
	}
	return (0);
}

int	count_tokens(const char *str, char delim)
{
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	in_quotes = 0;
	quote_char = 0;
	while (*str)
	{
		if (handle_quotes(&str, &in_quotes, &quote_char))
			continue ;
		if (!in_quotes && *str == delim)
		{
			count++;
			while (*str == delim)
				str++;
			continue ;
		}
		str++;
	}
	return (count + 1);
}

char	*get_next_token(char **str, char delim)
{
	const char	*start;
	char		*token;
	int			in_quotes;
	char		quote_char;

	in_quotes = 0;
	quote_char = 0;
	while (**str && (**str == delim || **str == ' ' || **str == '\t'))
		(*str)++;
	start = *str;
	while (**str)
	{
		if (handle_quotes((const char **)str, &in_quotes, &quote_char))
			continue ;
		if (!in_quotes && **str == delim)
			break ;
		(*str)++;
	}
	token = malloc((*str - start) + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, (*str - start) + 1);
	return (token);
}

char	**allocate_tokens(char *str, char delim, int token_count)
{
	char	**result;
	int		i;

	result = malloc((token_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < token_count)
	{
		result[i] = get_next_token(&str, delim);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split_shell(char *str, char delim)
{
	int	token_count;

	if (!str)
		return (NULL);
	token_count = count_tokens(str, delim);
	return (allocate_tokens(str, delim, token_count));
}
