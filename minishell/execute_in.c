/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:19 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/26 19:55:46 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int	i;

	char	*builtins[] ={
	"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
	i = 0;
	if (!cmd)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// char	*ft_strrechr(char *s, char *c)
// {
// 	char	*ch;
// 	int		i;

// 	i = 0;
// 	ch = c;
// 	while (s[i] != '\0')
// 	{
// 		if (s == ch)
// 			return (s);
// 		i++;
// 	}
// 	if (ch == NULL)
// 		return ((char *)s);
// 	return (NULL);
// }

int	handle_common_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "echo") == 0)
	{
		ft_echo(cmd, shell);
		return (exit_status = 0);
	}
	else if (ft_strcmp(command, "pwd") == 0)
	{
		exit_status = ft_pwd(cmd);
		return (exit_status);
	}
	else if (ft_strcmp(command, "env") == 0)
	{
		
		exit_status = ft_env(shell);
		return exit_status;
	}
	// else if (ft_split(command, '$') == 0)
	// {
	//     printf("innn\n");
	//     //int i = 1;
	//     ft_echo(cmd, shell);
	//     // print_env_var(command, &command, shell, cmd->out_fd);
	//     // process_echo_arg(command, shell, cmd->out_fd);
	//     //     if (cmd->args[i + 1])
	//     //     ft_putchar_fd(' ', cmd->out_fd);
	//     return (shell->exit_status = 0);
	// }
	return (0);
}

int	handle_special_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "cd") == 0)
	{
		exit_status = ft_cd(cmd, shell);
		return (exit_status);
	}
	else if (ft_strcmp(command, "export") == 0)
	{
		exit_status = ft_export(cmd, shell);
		return (exit_status);
	}
	else if (ft_strcmp(command, "unset") == 0)
	{
		exit_status = ft_unset(cmd, shell);
		return (0);
	}
	else if (ft_strcmp(command, "exit") == 0)
	{
		ft_exit(cmd, shell);
		return (1);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	command = cmd->args[0];
	if (handle_common_builtins(command, cmd, shell))
		return (exit_status);
	return (handle_special_builtins(command, cmd, shell));
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	int i = 0;
	int x = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	while(shell->input[i])
	{
			printf("HHHHHH   [%c]\n",shell->input[i]);
			if (shell->input[i] == '|' || shell->input[i] == '>' || shell->input[i] == '<')
			{
				x = 1;
				break;
			}
			i++;
	}
	 // Check for malformed commands with unclosed quotes
	 printf("hello %d\n", x);
	// If there's only one command and it's a builtin, execute directly
	if (cmd->next == NULL)
		//printf("HHHHHH\n");
	if ( !cmd->next && ft_strcmp(cmd->args[0], "cd") == 0)
		return(execute_builtin(cmd, shell));
	else if (!cmd->next && (is_builtin(cmd->args[0]) && x == 0 ))
	{
		execute_builtin(cmd, shell);
		return (0);
	}
	//else
	// Otherwise execute as pipeline
	return execute_pipeline(cmd, shell);
	//return 0;
}
