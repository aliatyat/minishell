/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:24:59 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 15:33:47 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(char **env, int fd)
{
	int			i;
	int			count;
	char		**sorted_env;

	i = 0;
	count = 0;
	while (env[count])
		count++;
	sorted_env = copy_env(env);
	if (!sorted_env)
		return ;
	sort_env(sorted_env, count);
	i = 0;
	while (sorted_env[i])
	{
		if (ft_strchr(sorted_env[i], '='))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(sorted_env[i], fd);
		}
		i++;
	}
	close(fd);
	free_split(sorted_env);
}
