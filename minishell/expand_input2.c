/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:54:17 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 19:27:46 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_input(char *input, t_shell *shell)
{
	char	buffer[4096];

	if (!input)
		return (NULL);
	expand_loop(input, buffer, shell);
	free (input);
	return (ft_strdup(buffer));
}

void	expand_loop(char *input, char *buffer, t_shell *shell)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;
	char	*ptr;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	ptr = input;
	while (*ptr && i < 4095)
	{
		if (handle_quotes_in(*ptr, &in_squote, &in_dquote))
		{
			ptr++;
			continue ;
		}
		if (*ptr == '$' && !in_squote && *(ptr + 1))
		{
			expand_dollar_in(&ptr, buffer, &i, shell);
			continue ;
		}
		buffer[i++] = *ptr++;
	}
	buffer[i] = '\0';
}
