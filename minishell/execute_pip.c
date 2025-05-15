/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:18:13 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:24:58 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_children(t_shell *shell)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == shell->last_fork)
		{
			if (WIFEXITED(status))
			{
				g_signal_status = -1;
				shell->exit_status = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				g_signal_status = 128 + WTERMSIG(status);
				shell->exit_status = -1;
			}
		}
		pid = wait(&status);
	}
}

int	check_heredoc_flag(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (ft_strncmp(cmd->args[i], "<<", 2) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	parent_process(int *prev_pipe_in, t_command *cmd, int heredoc_flag,
		t_shell *shell)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (*prev_pipe_in != -1)
		close(*prev_pipe_in);
	if (heredoc_flag)
		wait_for_children(shell);
	parent_cleanup(prev_pipe_in, cmd);
}

int	process_pipeline_stage(t_pipe_var *var, int *prev_pipe_in, int pipefd[2],
		t_shell *shell)
{
	pid_t	pid;

	if (start_pipe(var->current, var->next, pipefd))
		return (1);
	pid = fork();
	shell->last_fork = pid;
	if (pid < 0)
	{
		ft_perror("fork", 1);
		return (1);
	}
	if (pid == 0)
	{
		execute_child(var->current, *prev_pipe_in, shell);
	}
	else
	{
		parent_process(prev_pipe_in, var->current, var->f, shell);
	}
	if (var->next != NULL)
		*prev_pipe_in = pipefd[0];
	else
		*prev_pipe_in = -1;
	return (0);
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int			pipefd[2];
	int			prev_pipe_in;
	t_pipe_var	var;

	var.current = cmd;
	prev_pipe_in = -1;
	while (var.current)
	{
		var.next = var.current->next;
		var.f = check_heredoc_flag(var.current);
		if (process_pipeline_stage(&var, &prev_pipe_in, pipefd, shell))
			return (1);
		var.current = var.next;
	}
	wait_for_children(shell);
	return (shell->exit_status);
}
