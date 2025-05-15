/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pip2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:52:18 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:08:06 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_pipe(t_command *current, t_command *next, int *pipefd)
{
	if (next == NULL)
		return (0);
	if (pipe(pipefd) == -1)
	{
		ft_perror("pipe", 1);
		return (1);
	}
	current->out_fd = pipefd[1];
	next->in_fd = pipefd[0];
	return (0);
}

void	handle_redirections(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (has_redirection(cmd->args[i]))
		{
			cmd->args = normalize_segment(cmd->args, 0,
					count_tokens2(cmd->args));
			if (handle_redirection1(cmd, shell) == -1)
				exit(127);
		}
		i++;
	}
}

void	ft_setup(t_command *cmd, int prev_pipe_in, t_shell *shell)
{
	if (prev_pipe_in != -1)
		dup2(prev_pipe_in, STDIN_FILENO);
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
	close_all_pipes(cmd);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	else
		exit(execute_external(cmd, shell));
}

void	execute_child(t_command *cmd, int prev_pipe_in, t_shell *shell)
{
	handle_redirections(cmd, shell);
	ft_setup(cmd, prev_pipe_in, shell);
}

void	parent_cleanup(int *prev_pipe_in, t_command *cmd)
{
	if (*prev_pipe_in != -1)
		close(*prev_pipe_in);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}
