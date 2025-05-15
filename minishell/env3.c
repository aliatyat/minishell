/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:33:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:24:22 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**remove_env_var(char **env, char *var)
{
	size_t	var_len;
	int		count;
	char	**new_env;

	var_len = ft_strlen(var);
	count = count_env_vars(env);
	new_env = create_filtered_env(env, var, var_len, count);
	if (!new_env)
		return (NULL);
	free_split(env);
	return (new_env);
}

char	**cpy_env(char **env)
{
	char	**copy;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (env[count])
		count++;
	copy = malloc((count + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (i-- > 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

void	sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_with_prefix(char **env, int fd)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(env[i], fd);
		}
		i++;
	}
}

void	print_sorted_env(char **env, int fd)
{
	int		count;
	char	**sorted_env;

	count = 0;
	if (!env)
		return ;
	while (env[count])
		count++;
	sorted_env = cpy_env(env);
	if (!sorted_env)
		return ;
	sort_env(sorted_env, count);
	print_env_with_prefix(sorted_env, fd);
	free_split(sorted_env);
}
