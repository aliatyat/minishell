/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:12:31 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 15:50:14 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*create_command(char *input)
{
	t_command	*cmd;
	char		**tokens;
	int			i;

	i = 0;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->pipefd[0] = -1; // Initialize pipe file descriptors
	cmd->pipefd[1] = -1;
	tokens = ft_split_shell(input, ' ');
	if (!tokens)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = tokens;
	// Handle redirections
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], ">") == 0 && cmd->args[i + 1])
		{
			cmd->out_fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			cmd->args[i] = NULL;
		}
		else if (ft_strcmp(cmd->args[i], ">>") == 0 && cmd->args[i + 1])
		{
			cmd->out_fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_APPEND,
					0644);
			cmd->args[i] = NULL;
		}
		else if (ft_strcmp(cmd->args[i], "<") == 0 && cmd->args[i + 1])
		{
			cmd->in_fd = open(cmd->args[i + 1], O_RDONLY);
			cmd->args[i] = NULL;
		}
		i++;
	}
	return (cmd);
}

t_command	*parse_input(char *input, t_shell *shell)
{
	t_command	*head;
	t_command	*prev;
	char		**pipe_commands;
	int			i;
	t_command	*cmd;

	head = NULL;
	prev = NULL;
	i = 0;
	(void)shell;
	pipe_commands = ft_split_pipes(input);
	if (!pipe_commands)
		return (NULL);
	while (pipe_commands[i])
	{
		cmd = create_command(pipe_commands[i]);
		if (!cmd)
		{
			free_split(pipe_commands);
			free_commands(head);
			return (NULL);
		}
		if (!head)
			head = cmd;
		else
			prev->next = cmd;
		prev = cmd;
		i++;
	}
	free_split(pipe_commands);
	return (head);
}

void	setup_pipes(t_command *cmd)
{
	int			pipefd[2];
	t_command	*tmp;

	tmp = cmd;
	while (tmp && tmp->next)
	{
		if (pipe(pipefd) == -1)
		{
			ft_perror("pipe", 1);
			return ;
		}
		// Set output of current command to pipe write end
		if (tmp->out_fd != STDOUT_FILENO)
			close(tmp->out_fd);
		tmp->out_fd = pipefd[1];
		// Set input of next command to pipe read end
		if (tmp->next->in_fd != STDIN_FILENO)
			close(tmp->next->in_fd);
		tmp->next->in_fd = pipefd[0];
		tmp = tmp->next;
	}
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	pid_t pid;
	int prev_pipe_in = -1;
	int status;
	t_command *current = cmd;

	while (current)
	{
		if (current->next && pipe(current->pipefd) == -1)
		{
			ft_perror("pipe", 1);
			return (1);
		}

		pid = fork();
		if (pid == 0)
		{
			// Child process
			if (prev_pipe_in != -1)
			{
				dup2(prev_pipe_in, STDIN_FILENO);
				close(prev_pipe_in);
			}
			if (current->next)
			{
				dup2(current->pipefd[1], STDOUT_FILENO);
				close(current->pipefd[1]);
			}

			// Handle file redirections
			if (current->in_fd != STDIN_FILENO)
			{
				dup2(current->in_fd, STDIN_FILENO);
				close(current->in_fd);
			}
			if (current->out_fd != STDOUT_FILENO)
			{
				dup2(current->out_fd, STDOUT_FILENO);
				close(current->out_fd);
			}

			close_all_pipes(shell->commands);

			if (is_builtin(current->args[0]))
				exit(execute_builtin(current, shell));
			else
				exit(execute_external(current, shell));
		}
		else if (pid < 0)
		{
			ft_perror("fork", 1);
			return (1);
		}

		// Parent closes unused pipe ends
		if (prev_pipe_in != -1)
			close(prev_pipe_in);
		if (current->next)
			close(current->pipefd[1]);

		prev_pipe_in = current->pipefd[0];
		current = current->next;
	}

	// Wait for all children
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}

	if (prev_pipe_in != -1)
		close(prev_pipe_in);

	return (shell->exit_status);
}