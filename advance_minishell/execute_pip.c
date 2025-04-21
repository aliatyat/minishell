/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:18:13 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/21 16:36:50 by alalauty         ###   ########.fr       */
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

void	execute_child(t_command *cmd, int prev_pipe_in, t_shell *shell)
{
	printf("YOU IN\n");
	if (handle_redirection1(cmd) == -1)
	{
		free(handle_redirection1);
		return ;
	}
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		//close(prev_pipe_in);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		//close(cmd->out_fd);
	}
	close_all_pipes(shell->commands);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	else
		exit(execute_external(cmd, shell));
}

void	parent_cleanup(int *prev_pipe_in, t_command *cmd)
{
	printf("IN PPP\n");
	if (*prev_pipe_in != -1)
		close(*prev_pipe_in);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}

void	wait_for_children(t_shell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int			pipefd[2];
	pid_t		pid;
	t_command	*current;
	t_command	*next;
	int			prev_pipe_in;

	current = cmd;
	prev_pipe_in = -1;
	while (current)
	{
		next = current->next;
		if (start_pipe(current, next, pipefd))
			return (1);
		pid = fork();
		if (pid < 0)
			return (ft_perror("fork", 1), 1);
		if (pid == 0)
			execute_child(current, prev_pipe_in, shell);
		else if (pid != 0)
			wait_for_children(shell);
		parent_cleanup(&prev_pipe_in, current);
		if (next != NULL)
			prev_pipe_in = pipefd[0];
		else
			prev_pipe_in = -1;
		current = next;
	}
	return (shell->exit_status);
}
