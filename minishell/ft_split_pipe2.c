/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:41:28 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 18:42:25 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	have_herdocs(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strnstr(cmd, "<<", ft_strlen(cmd)))
		return (1);
	return (0);
}

void	prioritize_heredocs(char **cmds, int count)
{
	int		x;
	int		swapped;
	char	*swap;

	swapped = 0;
	x = 1;
	while (swapped)
	{
		swapped = 0;
		x = 1;
		while (x < count)
		{
			if (have_herdocs(cmds[x]) && !have_herdocs(cmds[x - 1]))
			{
				swap = cmds[x - 1];
				cmds[x - 1] = cmds[x];
				cmds[x] = swap;
				swapped = 1;
			}
			x++;
		}
	}
}
