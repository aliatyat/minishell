/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:38:37 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/11 14:03:44 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *s)
{
	return (!ft_strncmp(s, ">", 1) || !ft_strncmp(s, ">>", 2) || !ft_strncmp(s,
			"<", 1) || !ft_strncmp(s, "<<", 2));
}

int	count_tokens2(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

int	is_redirection_token(char *token)
{
	if (!token)
		return (0);
	if (ft_strlen(token) == 1)
		return (1);
	if (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0)
		return (1);
	return (0);
}

int	has_unclosed_quotes(char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	while (*str)
	{
		if (!in_quotes && (*str == '\'' || *str == '"'))
		{
			quote_char = *str;
			in_quotes = 1;
		}
		else if (in_quotes && *str == quote_char)
		{
			in_quotes = 0;
		}
		str++;
	}
	return (in_quotes);
}
