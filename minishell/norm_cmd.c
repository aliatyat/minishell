/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:30:23 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/13 20:07:43 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_red(char *s1, char *s2, int *i)
{
	char	*temp;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	(*i)++;
	return (temp);
}

int	count_heredocs(char **tokens)
{
	int	count;
	int	xr;

	count = 0;
	xr = 0;
	while (tokens && tokens[xr])
	{
		if (ft_strncmp(tokens[xr], "<<", 2) == 0)
			count++;
		xr++;
	}
	return (count);
}

void	shift_tokens_if_first_is_lt_and_has_heredoc(char **tokens)
{
	int	xr;
	int	count;
	int	total;

	if (!tokens || !tokens[0])
		return ;
	xr = 0;
	count = count_heredocs(tokens);
	if (ft_strncmp(tokens[0], "<", 1) == 0 && ft_strncmp(tokens[0], "<<",
			2) != 0 && count >= 1)
	{
		xr = 0;
		total = count_tokens2(tokens);
		free(tokens[0]);
		while (xr < total - 1)
		{
			tokens[xr] = tokens[xr + 1];
			xr++;
		}
		tokens[xr] = NULL;
	}
}

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
}

char	**normalize_segment(char **tokens, int start, int end)
{
	char	**args;
	char	**redir;
	char	*cmd;

	args = malloc(sizeof(char *) * (end - start + 10));
	redir = malloc(sizeof(char *) * (end - start + 10));
	if (!args || !redir)
		return (NULL);
	cmd = NULL;
	shift_tokens_if_first_is_lt_and_has_heredoc(tokens);
	fill_args_and_redir(tokens, args, redir, &cmd);
	free_tokens(tokens);
	tokens = gather_back(tokens, cmd, args, redir);
	free(redir);
	free(args);
	if (!cmd)
		return (tokens);
	return (tokens);
}
