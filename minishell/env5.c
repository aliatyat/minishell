/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:01:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:06:34 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**remove_null_args(char **args)
{
	int		count;
	int		i;
	char	**new_args;
	int		j;

	count = 0;
	j = 0;
	i = 0;
	while (args[count])
		count++;
	new_args = malloc((count + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	while (i < count)
	{
		if (args[i])
		{
			new_args[j] = args[i];
			j++;
		}
		i++;
	}
	new_args[j] = NULL;
	free(args);
	return (new_args);
}

char	*find_path(char **env, char *cmd)
{
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = get_paths_from_env(env);
	if (!paths)
		return (NULL);
	return (search_path_in_env(paths, cmd));
}
