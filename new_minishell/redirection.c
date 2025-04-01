#include "minishell.h"

int handle_redirection1(char **cmd)
{
    int i = 0;
    int input_fd = -1;
    int output_fd = -1;

    while (cmd[i])
    {
        if (ft_strcmp(cmd[i], ">") == 0)
        {
            output_fd = open(cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd); // Close after duplicating
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
        }
        else if (ft_strcmp(cmd[i], ">>") == 0)
        {
            output_fd = open(cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd); // Close after duplicating
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
        }
        else if (ft_strcmp(cmd[i], "<") == 0)
        {
            input_fd = open(cmd[i + 1], O_RDONLY);
            if (input_fd == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd); // Close after duplicating
            cmd[i] = NULL;
            cmd[i + 1] = NULL;
        }
        else if (ft_strcmp(cmd[i], "<<") == 0)
        {
            // Handle here-document (heredoc)
            // This is a more complex case and requires additional handling
        }
        i++;
    }

    // Remove NULL entries from the command array
    i = 0;
    int j = 0;
    while (cmd[i])
    {
        if (cmd[i] != NULL)
        {
            cmd[j++] = cmd[i];
        }
        i++;
    }
    cmd[j] = NULL;
    return(0);
}

int has_redirection(char *cmd)
{
    int i = 0;
    
    while (cmd[i])
    {
        if (cmd[i] == '>' || cmd[i] == '<')
        {
            if (cmd[i] == '>' && cmd[i + 1] == '>')
                return 1;
            return 1;
        }
        i++;
    }
    
    return 0;
}