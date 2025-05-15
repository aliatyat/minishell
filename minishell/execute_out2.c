/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_out2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:28:48 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 21:24:52 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_file_descriptors(t_command *cmd)
{
	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			ft_perror("minishell: dup2", 1);
			return (1);
		}
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			ft_perror("minishell: dup2", 1);
			return (1);
		}
		close(cmd->out_fd);
	}
	return (0);
}

void	cmd_args(t_command *cmd)
{
	int	x;
	int	has_space;

	x = 0;
	has_space = 0;
	while (cmd->args[0][x])
	{
		if (cmd->args[0][x] == ' ')
			has_space = 1;
		x++;
	}
	if (has_space)
		cmd->args = ft_split(cmd->args[0], ' ');
}

void	exe_cmd(char *full_path, t_command *cmd, t_shell *shell)
{
	char	**env_array;

	env_array = env_to_array(shell->env);
	if (!env_array)
	{
		free(full_path);
		exit(EXIT_FAILURE);
	}
	execve(full_path, cmd->args, env_array);
	free(full_path);
	free_split(env_array);
	ft_perror(cmd->args[0], 127);
	exit(127);
}

void	execute_command_in_child(t_command *cmd, t_shell *shell,
		char *full_path)
{
	if (!cmd->args || !cmd->args[0])
		return ;
	cmd_args(cmd);
	exe_cmd(full_path, cmd, shell);
}

int	handle_child_process(t_command *cmd, t_shell *shell, char *full_path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_file_descriptors(cmd) != 0)
		exit(1);
	close_all_pipes(cmd);
	execute_command_in_child(cmd, shell, full_path);
	return (0);
}
