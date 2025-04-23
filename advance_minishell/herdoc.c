/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/23 14:31:57 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_command *cmd, char *delimiter)
{
	int	pipe_fd[2];
	char	*line;

	line = NULL;
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell");
		return (-1);
	}

	signal(SIGINT, SIG_DFL);  // Reset signals for heredoc
    signal(SIGQUIT, SIG_DFL);
	
	// Write heredoc content to pipe
	while (1)
	{
		
		ft_putstr_fd("> ", STDOUT_FILENO);  // Heredoc prompt
		 line = get_next_line(STDIN_FILENO); // Read from stdin
		 if (line[0] == '\0')
		 {
			 free(line);
			 break ;
		 }
		//line = readline("> ");
		printf("{%s}\n (%s)\n %d\n", delimiter, line, cmd->in_fd);
		if (!line || (ft_strlen(line) == ft_strlen(delimiter) + 1
				&& ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& line[ft_strlen(delimiter)] == '\n'))
		{
			free(line);
			break ;
		}
		//ft_echo(cmd, NULL);
		write(pipe_fd[1], line, ft_strlen(line));
			
		free(line);
	}
	close(pipe_fd[1]); // Close write end
	// Set input fd to pipe's read end
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	cmd->in_fd = pipe_fd[0];
	printf("CMD %d\n", cmd->in_fd);
	//expand_input(line, (t_shell*)cmd);
	//dup2(cmd->in_fd, STDIN_FILENO);
	return (0);
	
}
