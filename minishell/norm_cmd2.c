/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:44:28 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 15:46:40 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**gather_back(char **tokens, char *cmd, char **args, char **redir)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (cmd)
		tokens[i++] = cmd;
	while (args[j])
	{
		tokens[i++] = args[j++];
	}
	j = 0;
	while (redir[j])
	{
		tokens[i++] = redir[j++];
	}
	tokens[i] = NULL;
	return (tokens);
}

void	null_terminate(char **redir, char **argc, int end_argc, int end_redir)
{
	redir[end_redir] = NULL;
	argc[end_argc] = NULL;
}

int	protect_redir(char *red, char **redir, char **args)
{
	if (red == NULL)
	{
		free_2d_array(redir);
		free_2d_array(args);
		return (-1);
	}
	return (0);
}

void	fill_args_and_redir(char **tokens, char **args, char **redir,
		char **cmd)
{
	int	n[3];

	ft_memset(n, 0, sizeof(n));
	while (n[2] < count_tokens2(tokens))
	{
		if (is_redirection(tokens[n[2]]) && (n[2] < count_tokens2(tokens)))
		{
			if (is_redirection_token(tokens[n[2]]) && tokens[n[2] + 1])
			{
				redir[n[1]] = join_red(tokens[n[2]], tokens[n[2] + 1], &n[2]);
				if (protect_redir(redir[n[1]], redir, args) == -1)
					return ;
				n[1]++;
			}
			else
				redir[n[1]++] = ft_strdup(tokens[n[2]]);
		}
		else if (!(*cmd))
			*cmd = ft_strdup(tokens[n[2]]);
		else
			args[n[0]++] = ft_strdup(tokens[n[2]]);
		n[2]++;
	}
	null_terminate(redir, args, n[0]++, n[1]++);
}
