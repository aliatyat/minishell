/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:42:58 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 19:57:02 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_special_characters(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (shell->input[i] == '|' || shell->input[i] == '>'
			|| shell->input[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	int	x;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	x = check_special_characters(shell);
	if (cmd->next == NULL)
	{
		if (ft_strcmp(cmd->args[0], "cd") == 0)
			return (execute_builtin(cmd, shell));
		if (is_builtin(cmd->args[0]) && x == 0)
		{
			execute_builtin(cmd, shell);
			return (shell->exit_status);
		}
	}
	close_all_pipes(cmd);
	return (execute_pipeline(cmd, shell));
}
