/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:53:41 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 19:27:55 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes_in(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
	{
		*in_squote = !(*in_squote);
		return (1);
	}
	if (c == '"' && !(*in_squote))
	{
		*in_dquote = !(*in_dquote);
		return (1);
	}
	return (0);
}

static void	expand_shell_name(char *buffer, size_t *i, t_shell *shell)
{
	ft_strlcpy(buffer + *i, shell->argv_z, 4096 - *i);
	*i += ft_strlen(shell->argv_z);
}

static void	expand_exit_status(char *buffer, size_t *i, t_shell *shell)
{
	char	*exit_status;

	if (g_signal_status != -1)
		exit_status = ft_itoa(g_signal_status);
	else
		exit_status = ft_itoa(shell->exit_status);
	if (exit_status)
	{
		ft_strlcpy(buffer + *i, exit_status, 4096 - *i);
		*i += ft_strlen(exit_status);
		free(exit_status);
	}
}

static void	expand_variable(char **ptr, char *buffer, size_t *i, t_shell *shell)
{
	char	*var_start;
	char	*var_name;
	char	*value;
	int		len;

	var_start = *ptr;
	len = 0;
	while (ft_isalnum(var_start[len]) || var_start[len] == '_')
		len++;
	var_name = ft_substr(var_start, 0, len);
	value = get_env_value(shell->env, var_name);
	if (value)
	{
		ft_strlcpy(buffer + *i, value, 4096 - *i);
		*i += ft_strlen(value);
		free(value);
	}
	free(var_name);
	*ptr += len;
}

void	expand_dollar_in(char **ptr, char *buffer, size_t *i, t_shell *shell)
{
	(*ptr)++;
	if (**ptr == '?')
	{
		expand_exit_status(buffer, i, shell);
		(*ptr)++;
	}
	else if (**ptr == '0')
	{
		expand_shell_name(buffer, i, shell);
		(*ptr)++;
	}
	else if (ft_isalpha(**ptr) || **ptr == '_')
		expand_variable(ptr, buffer, i, shell);
	else
		buffer[(*i)++] = '$';
}
