/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:03 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:25:53 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_command(char *input, t_shell *shell)
{
	t_command	*cmd;
	char		**tokens;

	(void)shell;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	tokens = ft_split_shell(input, ' ');
	if (!tokens)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = tokens;
	return (cmd);
}

char	**pipe_split(char *input)
{
	char	**pipe_cmd;

	if (!input || input[0] == '\0')
		return (NULL);
	pipe_cmd = ft_split_pipes(input);
	if (!pipe_cmd)
	{
		free_split(pipe_cmd);
		return (NULL);
	}
	return (pipe_cmd);
}

t_command	*cmd_list(char **pipe_cmd, t_shell *shell)
{
	t_command	*head;
	t_command	*prev;
	t_command	*cmd;
	int			i;

	head = NULL;
	prev = NULL;
	i = 0;
	while (pipe_cmd[i])
	{
		cmd = create_command(pipe_cmd[i], shell);
		if (!cmd)
		{
			free_split(pipe_cmd);
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
	return (head);
}

t_command	*parse_input(char *input, t_shell *shell)
{
	char		**pipe_cmd;
	t_command	*head;

	pipe_cmd = NULL;
	head = NULL;
	if (!input || input[0] == '\0')
		return (NULL);
	if (input && is_invalid_syntax(input) != 0)
	{
		shell->exit_status = 2;
		return (NULL);
	}
	pipe_cmd = pipe_split(input);
	if (!pipe_cmd)
		return (NULL);
	head = cmd_list(pipe_cmd, shell);
	free_split(pipe_cmd);
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
		if (tmp->out_fd != STDOUT_FILENO)
			close(tmp->out_fd);
		tmp->out_fd = pipefd[1];
		if (tmp->next->in_fd != STDIN_FILENO)
			close(tmp->next->in_fd);
		tmp->next->in_fd = pipefd[0];
		tmp = tmp->next;
	}
	if (tmp && tmp->out_fd != STDOUT_FILENO)
		close(tmp->out_fd);
	if (tmp && tmp->in_fd != STDIN_FILENO)
		close(tmp->in_fd);
}
