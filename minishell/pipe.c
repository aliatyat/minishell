/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:03 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/28 17:50:21 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char **copy_args(char **args)
// {
//     char **new_args;
//     int count = 0;
//     int i = 0;

//     while (args[count])
//         count++;

//     new_args = malloc((count + 1) * sizeof(char *));
//     if (!new_args)
//         return (NULL);

//     while (i < count)
//     {
//         new_args[i] = ft_strdup(args[i]);
//         if (!new_args[i])
//         {
//             while (i-- > 0)
//                 free(new_args[i]);
//             free(new_args);
//             return (NULL);
//         }
//         i++;
//     }
//     new_args[i] = NULL;
//     return (new_args);
// }

t_command	*create_command(char *input, t_shell *shell)
{
	t_command	*cmd;
	char		**tokens;
	(void)shell;
    // // First expand variables in the input string
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	tokens = ft_split_shell(input, ' ');
	if (!tokens)
		return NULL;
	if (!tokens)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = tokens;
	return (cmd);
}

t_command	*parse_input(char *input, t_shell *shell)
{
	t_command	*head;
	t_command	*prev;
	char		**pipe_commands = NULL;
	int			i;
	t_command	*cmd;

	head = NULL;
	prev = NULL;
	i = 0;
	char *expanded_input;

	char *command = input;
    if (has_unclosed_quotes(command))
    {
		shell->exit_status = 127;
        ft_perror("unclosed quotes\n", 127);
        return NULL;
    }
   if (command[0] == '|')
    {
		exit_status = 2;
        ft_perror("syntax error near unexpected token `|'\n", 2);
        return (NULL);
    }
	//(void)shell;
	char *pipefound = ft_strdup(input);
	
	while (pipefound[i])
	{
		
		if (pipefound[i + 1] && (pipefound[i] == '|' || pipefound[i] == '-') &&   pipefound[0] == '"' ) 
		{
			printf("FUCK\n");
			return (NULL);
		}
		i++;
	}
	i = 0;
	char **input1 = split_tokens(input);
	pipe_commands = normalize_command(input1);
	input = join_tokens_back(pipe_commands);
	printf("ZAID: %s\n", input);
	 // First expand variables in the input string
    expanded_input = expand_input(input, shell);
	printf("EXP: %s\n", expanded_input);
	pipe_commands = ft_split_pipes(expanded_input);
	i = 0;
	while(pipe_commands[i])
	{
		printf("ALI: %s\n", pipe_commands[i]);
		i++;
	}
	if (!expanded_input)
	{
		free(expanded_input);
		return NULL;
	}
	i = 0;
	if (!pipe_commands)
		return (NULL);
	while (pipe_commands[i])
	{
		cmd = create_command(pipe_commands[i], shell);
		int x = 0;
		while(cmd->args[x])
		{
			printf("COMMAND: %s\n", cmd->args[x]);
			x++;	
		}
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

// int	execute_pipeline(t_command *cmd, t_shell *shell)
// {
// 	int pipefd[2];
// 	pid_t pid;
// 	t_command *current = cmd;
// 	t_command *next;
// 	int prev_pipe_in = -1;
// 	int status;

// 	while (current)
// 	{
// 		next = current->next;

// 		if (next)
// 		{
// 			if (pipe(pipefd) == -1)
// 			{
// 				ft_perror("pipe", 1);
// 				return (1);
// 			}
// 			current->out_fd = pipefd[1];
// 			next->in_fd = pipefd[0];
// 		}

// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			// Child process
// 			if (prev_pipe_in != -1)
// 			{
// 				dup2(prev_pipe_in, STDIN_FILENO);
// 				close(prev_pipe_in);
// 			}
// 			if (current->out_fd != STDOUT_FILENO)
// 			{
// 				dup2(current->out_fd, STDOUT_FILENO);
// 				close(current->out_fd);
// 			}

// 			// Close all other pipe ends
// 			close_all_pipes(shell->commands);

// 			if (is_builtin(current->args[0]))
// 				exit(execute_builtin(current, shell));
// 			else
// 				exit(execute_external(current, shell));
// 		}
// 		else if (pid < 0)
// 		{
// 			ft_perror("fork", 1);
// 			return (1);
// 		}

// 		// Parent closes unused pipe ends
// 		if (prev_pipe_in != -1)
// 			close(prev_pipe_in);
// 		if (current->out_fd != STDOUT_FILENO)
// 			close(current->out_fd);

// 		prev_pipe_in = next ? pipefd[0] : -1;
// 		current = next;
// 	}

// 	// Wait for all children
// 	while (wait(&status) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			shell->exit_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			shell->exit_status = 128 + WTERMSIG(status);
// 	}

// 	return (shell->exit_status);
// }