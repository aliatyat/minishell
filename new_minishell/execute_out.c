#include "minishell.h"

/* Main external command executor */
// static int handle_child_process(t_command *cmd, t_shell *shell, char *full_path)
// {
//     char **env_array;

//     signal(SIGINT, SIG_DFL);
//     signal(SIGQUIT, SIG_DFL);

//     // Redirect input if necessary
//     if (cmd->in_fd != STDIN_FILENO)
//     {
//         dup2(cmd->in_fd, STDIN_FILENO);
//         close(cmd->in_fd); // Close the original file descriptor
//     }

//     // Redirect output if necessary
//     if (cmd->out_fd != STDOUT_FILENO)
//     {
//         dup2(cmd->out_fd, STDOUT_FILENO);
//         close(cmd->out_fd); // Close the original file descriptor
//     }

//     close_all_pipes(shell->commands); // Ensure all unused pipes are closed

//     env_array = env_to_array(shell->env);
//     if (!env_array)
//     {
//         free(full_path);
//         exit(EXIT_FAILURE);
//     }

//     execve(full_path, cmd->args, env_array);

//     // If execve fails
//     free(full_path);
//     free_split(env_array);
//     ft_perror(cmd->args[0], 127);
//     exit(127);
// }

static int handle_parent_process(pid_t pid, t_shell *shell)
{
    int status;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    waitpid(pid, &status, 0);
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
    
    return shell->exit_status;
}

#include "minishell.h"

int execute_external(t_command *cmd, t_shell *shell)
{
    pid_t pid;
    char *full_path;

    if (!cmd || !cmd->args || !cmd->args[0])
        return 0;

    full_path = find_path(shell->env, cmd->args[0]);
    if (!full_path)
    {
        ft_error(cmd->args[0], "command not found", 127);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        free(full_path);
        ft_perror("fork", 1);
        return 1;
    }

    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        // Handle input redirection
        if (cmd->in_fd != STDIN_FILENO)
        {
            dup2(cmd->in_fd, STDIN_FILENO);
            close(cmd->in_fd);
        }

        // Handle output redirection
        if (cmd->out_fd != STDOUT_FILENO)
        {
            dup2(cmd->out_fd, STDOUT_FILENO);
            close(cmd->out_fd);
        }

        // Close all other file descriptors
        close_all_pipes(shell->commands);

        // Execute the command
        execve(full_path, cmd->args, env_to_array(shell->env));

        // If execve fails
        free(full_path);
        ft_perror(cmd->args[0], 127);
        exit(127);
    }
    else
    {
        // Parent process
        free(full_path);
        return handle_parent_process(pid, shell);
    }
}

int execute_single_command(t_shell *shell, char *cmd_str)
{
    t_command *cmd = parse_input(cmd_str, shell);
    int status = 0;
    
    if (!cmd)
        return EXIT_FAILURE;
        
    status = execute_command(cmd, shell);
    free_commands(cmd);
    return status;
}