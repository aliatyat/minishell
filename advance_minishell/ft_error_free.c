#include "minishell.h"

int ft_error( char *cmd,  char *msg, int status)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putendl_fd(msg, STDERR_FILENO);
    return status;
}

int ft_perror( char *cmd, int status)
{
    perror("minishell");
    if (cmd)
    {
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
    }
    return status;
}

void free_commands(t_command *cmd)
{
    t_command *tmp;

    while (cmd)
    {
        tmp = cmd;
        cmd = cmd->next;
        free_split(tmp->args);
        if (tmp->in_fd != STDIN_FILENO)
            close(tmp->in_fd);
        if (tmp->out_fd != STDOUT_FILENO) 
            close(tmp->out_fd);
        free(tmp);
    }
}

void free_split(char **array)
{
    int i = 0;

    if (!array)
        return;
    
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void free_env(char **env)
{
    int i = 0;

    if (!env)
        return;
    
    while (env[i])
        free(env[i++]);
    free(env);
}



// Free a NULL-terminated array of strings
void free_2d_array(char **array)
{
    int i = 0;
    if (!array)
        return;
    
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
