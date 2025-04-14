#include "minishell.h"


int handle_heredoc(t_command *cmd, char *delimiter) 
{
    int pipe_fd[2];
    char *line = NULL;

    if (pipe(pipe_fd) == -1) 
    {
        perror("minishell");
        return -1;
    }

    // Write heredoc content to pipe
    while (1) 
    {
        ft_putstr_fd("> ", STDOUT_FILENO);  // Heredoc prompt
        line = get_next_line(STDIN_FILENO);  // Read from stdin
        
        if (!line || (ft_strlen(line) == ft_strlen(delimiter) + 1 && 
         ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 && 
         line[ft_strlen(delimiter)] == '\n')) 
        {
             free(line);
             break;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        free(line);
    }
    close(pipe_fd[1]);  // Close write end

    // Set input fd to pipe's read end
    if (cmd->in_fd != STDIN_FILENO)
        close(cmd->in_fd);
    cmd->in_fd = pipe_fd[0];

    return 0;
}