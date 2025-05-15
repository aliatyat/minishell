/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_unval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:59:30 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:23:16 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_or_ampersand(char *str, int *i)
{
	if (str[*i] == '|' || str[*i] == '&')
	{
		(*i)++;
		while (str[*i] == ' ' || str[*i] == '\t')
			(*i)++;
		if (!str[*i] || str[*i] == '|' || str[*i] == '&')
		{
			printf("minishell: not supported\n");
			return (2);
		}
	}
	return (0);
}

int	is_error(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (2);
	}
	while (str[i])
	{
		if (check_pipe_or_ampersand(str, &i) != 0)
			return (2);
		i++;
	}
	return (0);
}

int	is_invalid_syntax(char *input)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	i = 0;
	in_quotes = 0;
	quote_char = 0;
	if (!input || input[0] == '\0')
		return (1);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] && is_error(&input[i]) != 0)
			return (2);
		else
			return (0);
		i++;
	}
	return (0);
}
