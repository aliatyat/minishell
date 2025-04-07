/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:11:48 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 15:33:42 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc((i + 1) * sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (i-- > 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*get_env_value(char **env, char *name)
{
	size_t	name_len;
	int		i;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
			return (env[i] + name_len + 1);
	}
	return (NULL);
}

char	**update_env_var(char **env, char *name, const char *value)
{
	char	*new_var;
	size_t	name_len;
	int		i;

	if (!env || !name)
		return (env);
	name_len = ft_strlen(name);
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (env);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (env);
		}
	}
	env = ft_realloc_strarr(env, i + 2);
	if (!env)
	{
		free(new_var);
		return (NULL);
	}
	env[i] = new_var;
	env[i + 1] = NULL;
	return (env);
}

char	**remove_env_var(char **env, char *var)
{
	int		count;
	int		i;
	int		j;
	char	**new_env;
	size_t	var_len;

	count = 0;
	i = 0;
	j = 0;
	var_len = ft_strlen(var);
	while (env[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) != 0 || env[i][var_len] != '=')
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
			{
				while (j-- > 0)
					free(new_env[j]);
				free(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_split(env);
	return (new_env);
}

