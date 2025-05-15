/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redirections2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:40:08 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 18:42:19 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *s, size_t n)
{
	char	*new;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = '\0';
	return (new);
}

char	*skip_whitespace(char *ptr)
{
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	return (ptr);
}

char	*handle_redirection_token(char **ptr, int length)
{
	char	*token;

	token = ft_strndup(*ptr, length);
	*ptr += length;
	return (token);
}

char	*handle_quoted_string(char **ptr)
{
	char	quote;
	char	*start;

	start = *ptr;
	quote = **ptr;
	(*ptr)++;
	while (**ptr && **ptr != quote)
		(*ptr)++;
	if (**ptr == quote)
		(*ptr)++;
	return (ft_strndup(start, *ptr - start));
}
