/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:13 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:23:44 by alalauty         ###   ########.fr       */
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
	char	*xx;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			xx = ft_strtrim(env[i] + name_len + 1, "\"");
			return (xx);
		}
	}
	return (NULL);
}

int	update_existing_env_var(char **env, char *name, char *new_var)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (1);
		}
	}
	return (0);
}

char	**add_new_env_var(char **env, char *new_var, int i)
{
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

char	**update_env_var(char **env, char *name, char *value)
{
	char	*new_var;
	int		i;

	if (!env || !name)
		return (env);
	new_var = ft_strjoin4(name, "=", value);
	if (!new_var)
		return (env);
	if (update_existing_env_var(env, name, new_var))
		return (env);
	i = 0;
	while (env[i])
		i++;
	return (add_new_env_var(env, new_var, i));
}
