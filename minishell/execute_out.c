/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_out.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:22 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/28 14:07:32 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main external command executor */
int	handle_child_process(t_command *cmd, t_shell *shell, char *full_path)
{
	//printf("HHHHHHHHHHHHHH\n");
	char	**env_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	// if (handle_redirection1(cmd) == -1)
	// {
	// 	free(handle_redirection1);
	// 	return (1);
	// }
	if (cmd->in_fd != STDIN_FILENO)
    {
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
        {
            ft_perror("minishell: dup2", 1);
            return 1;
        }
        close(cmd->in_fd);
    }
	// Redirect input if necessary
	// if (cmd->in_fd != STDIN_FILENO)
	// {
	// 	dup2(cmd->in_fd, STDIN_FILENO);
	// 	close(cmd->in_fd); // Close the original file descriptor
	// }
	// Redirect output if necessary
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if(dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			ft_perror("minishell: dup2", 1);
            return 1;
		}
		close(cmd->out_fd); // Close the original file descriptor
	}
	close_all_pipes(shell->commands); // Ensure all unused pipes are closed
	env_array = env_to_array(shell->env);
	if (!env_array)
	{
		free(full_path);
		exit(EXIT_FAILURE);
	}
	//signal(SIGINT, SIG_IGN);
	execve(full_path, cmd->args, env_array);
	// if (cmd->in_fd == STDIN_FILENO)
	// {
	//     dup2(cmd->in_fd, STDIN_FILENO);
	//     close(cmd->in_fd); // Close the original file descriptor
	// }
	// If execve fails
	free(full_path);
	free_split(env_array);
	ft_perror(cmd->args[0], 127);
	exit(127);
}

int	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}

int	execute_external(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*full_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	full_path = find_path(shell->env, cmd->args[0]);
	if (!full_path)
	{
		ft_error(cmd->args[0], "command not found", 127);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(full_path);
		ft_perror("fork", 1);
		return (1);
	}
	if (pid == 0)
	{
		handle_child_process(cmd, shell, full_path);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		free(full_path);
		return (handle_parent_process(pid, shell));
	}
	if (cmd->in_fd == STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd); // Close the original file descriptor
	}
	//return (0); THE LAST UPDATA
	exit (0);
}

// int	execute_single_command(t_shell *shell, char *cmd_str)
// {
// 	t_command *cmd;
// 	int	status;

// 	cmd = parse_input(cmd_str, shell);
// 	status = 0;

// 	if (!cmd)
// 		return (EXIT_FAILURE);
// 	status = execute_command(cmd, shell);
// 	free_commands(cmd);
// 	return (status);
// }
