/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:10 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/17 21:36:21 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int handle_option(t_command *cmd, int *i)
// {
//     if (cmd->args[1] && !ft_strcmp(cmd->args[1], "-n"))
//     {
//         (*i)++;
//         return (0);
//     }
//     return (1);
// }

// void	print_exit_status1(t_shell *shell, int out_fd)
// {
// 	char	*exit_status;

// 	exit_status = ft_itoa(shell->exit_status);
// 	ft_putstr_fd(exit_status, out_fd);
// 	free(exit_status);
// }

// void	print_env_var1(char *var_start, char **arg, t_shell *shell, int out_fd)
// {
// 	char	*var_name;
// 	char	*value;

// 	while (**arg && (ft_isalnum(**arg) || **arg == '_'))
// 		(*arg)++;
// 	var_name = ft_substr(var_start, 0, *arg - var_start);
// 	value = get_env_value(shell->env, var_name);
// 	if (value)
// 		ft_putstr_fd(value, out_fd);
// 	free(var_name);
// }

// int	process_arg1(char *arg, t_shell *shell, int out_fd)
// {
// 	int	in_single_quote;
// 	int	in_double_quote;

// 	in_single_quote = 0;
// 	in_double_quote = 0;
// 	while (*arg)
// 	{
// 		if (*arg == '\'' && !in_double_quote)
// 		{
// 			in_single_quote = !in_single_quote;
// 			arg++;
// 			continue ;
// 		}
// 		else if (*arg == '"' && !in_single_quote)
// 		{
// 			in_double_quote = !in_double_quote;
// 			arg++;
// 			continue ;
// 		}
// 		if (*arg == '$' && *(arg + 1) && !in_single_quote)
// 		{
// 			arg++;
// 			if (*arg == '?')
// 			{
// 				print_exit_status1(shell, out_fd);
// 				arg++;
// 				continue ;
// 			}
// 			print_env_var1(arg, &arg, shell, out_fd);
// 		}
// 		else
// 		{
// 			ft_putchar_fd(*arg, out_fd);
// 			arg++;
// 		}
// 	}
// 	return (0);
// }

// char	*ft_ex(char *cmd, t_shell *shell)
// {
// 	int i = 1;
// 	//int newline = handle_option(cmd, &i);
// 	int out_fd = -1;

// 	while (cmd[i])
// 	{
// 		process_arg1(&cmd[i], shell, out_fd);
// 		if (cmd[i + 1])
// 			ft_putchar_fd(' ', out_fd);
// 		i++;
// 	}

	// if (newline)
	//     ft_putchar_fd('\n', out_fd);
// 	return (NULL);
// }

// #include "minishell.h"

char *expand_input(char *input, t_shell *shell)
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