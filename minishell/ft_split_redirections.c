/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:41 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:25:05 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_token(char **ptr)
{
	char	*start;
	char	quote;

	start = *ptr;
	quote = 0;
	while (**ptr && (quote || (**ptr != ' ' && **ptr != '\t' && **ptr != '>'
				&& **ptr != '<')))
	{
		if (**ptr == '"' || **ptr == '\'')
		{
			if (quote == 0)
				quote = **ptr;
			else if (quote == **ptr)
				quote = 0;
		}
		(*ptr)++;
	}
	return (ft_strndup(start, *ptr - start));
}

static char	**init_token_array(void)
{
	char	**tokens;
	int		i;

	tokens = malloc(100 * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < 100)
		tokens[i++] = NULL;
	return (tokens);
}

static int	add_token(char **tokens, int *i, char *token)
{
	if (!token)
	{
		free_split(tokens);
		return (-1);
	}
	tokens[(*i)++] = token;
	return (0);
}

static int	handle_next_token(char **ptr, char **tokens, int *i)
{
	char	*token;

	if (**ptr == '>' || **ptr == '<')
	{
		if ((*ptr)[1] == **ptr)
			token = handle_redirection_token(ptr, 2);
		else
			token = handle_redirection_token(ptr, 1);
	}
	else if (**ptr == '"' || **ptr == '\'')
		token = handle_quoted_string(ptr);
	else
		token = extract_token(ptr);
	return (add_token(tokens, i, token));
}

char	**split_with_redirections(char *input)
{
	char	*ptr;
	int		i;
	char	**tokens;

	tokens = init_token_array();
	if (!tokens)
		return (NULL);
	ptr = input;
	i = 0;
	while (*ptr)
	{
		ptr = skip_whitespace(ptr);
		if (!*ptr)
			break ;
		if (handle_next_token(&ptr, tokens, &i) == -1)
			return (NULL);
	}
	tokens[i] = NULL;
	return (tokens);
}
