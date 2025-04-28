/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/27 22:11:49 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_here(char *input, t_shell *shell)
{
    char buffer[4096];
    size_t buf_index = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;
    char *ptr = input;

    if (!input)
        return NULL;

    while (*ptr && buf_index < sizeof(buffer) - 1)
    {
        if (*ptr == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            buffer[buf_index++] = *ptr++;
            continue;
        }
        else if (*ptr == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            buffer[buf_index++] = *ptr++;
            continue;
        }

        if (*ptr == '$' && !in_single_quote && *(ptr + 1))
        {
            ptr++; // Skip $
            if (*ptr == '?')
            {
                char *exit_status = ft_itoa(shell->exit_status);
                if (exit_status)
                {
                    ft_strlcpy(buffer + buf_index, exit_status, sizeof(buffer) - buf_index);
                    buf_index += ft_strlen(exit_status);
                    free(exit_status);
                }
                ptr++;
                continue;
            }
            else if (ft_isalpha(*ptr) || *ptr == '_')
            {
                char *var_start = ptr;
                while (ft_isalnum(*ptr) || *ptr == '_')
                    ptr++;
                
                char *var_name = ft_substr(var_start, 0, ptr - var_start);
                char *value = get_env_value(shell->env, var_name);
                if (value)
                {
                    ft_strlcpy(buffer + buf_index, value, sizeof(buffer) - buf_index);
                    buf_index += ft_strlen(value);
                }
                free(var_name);
                continue;
            }
            // If not a valid variable, keep the $
            buffer[buf_index++] = '$';
        }
        else
        {
            buffer[buf_index++] = *ptr++;
        }
    }

    buffer[buf_index] = '\0';
    return ft_strdup(buffer);
}

int	handle_heredoc(t_command *cmd, char *delimiter, t_shell *shell)
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
		if (delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
			write(pipe_fd[1], line, ft_strlen(line));
		else
		{
			line = expand_here(line, shell);
		    write(pipe_fd[1], line, ft_strlen(line));
		}	
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
