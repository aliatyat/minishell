/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:12:37 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/07 14:45:39 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_command *parse_input(char *input, t_shell *shell)
// {
//     t_command *cmd = NULL;
//     char **tokens;
//     int i = 0;

//     (void)shell; // Mark unused parameter explicitly

//     tokens = ft_split_shell(input, ' '); // Now passing both arguments
//     if (!tokens)
//         return (NULL);

//     cmd = malloc(sizeof(t_command));
//     if (!cmd)
//     {
//         free_split(tokens);
//         return (NULL);
//     }

//     ft_memset(cmd, 0, sizeof(t_command));
//     cmd->args = tokens;
//     cmd->in_fd = STDIN_FILENO;
//     cmd->out_fd = STDOUT_FILENO;

//     // Handle redirections (simplified example)
//     while (cmd->args[i])
//     {
//         if (ft_strcmp(cmd->args[i], ">") == 0 && cmd->args[i+1])
//         {
//             cmd->out_fd = open(cmd->args[i+1], O_WRONLY|O_CREAT|O_TRUNC,
// 0644);
//             cmd->args[i] = NULL;
//         }
//         i++;
//     }

//     return (cmd);
// }

char	*ft_strndup(char *s, size_t n)
{
	char	*new;
	size_t	len;

	len = ft_strlen(s);
	// Use whichever is smaller: the string length or n
	if (n < len)
		len = n;
	new = malloc(len + 1); // +1 for null terminator
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len); // Copy up to n characters
	new[len] = '\0';        // Always null-terminate
	return (new);
}

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	**ft_realloc_strarr(char **arr, size_t new_size)
{
	char	**new;
	size_t	i;

	new = malloc(new_size * sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	if (arr)
	{
		while (arr[i] && i < new_size - 1)
		{
			new[i] = arr[i];
			i++;
		}
		free(arr);
	}
	while (i < new_size)
		new[i++] = NULL;
	return (new);
}

static int	count_tokens(char *str, char delim)
{
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	in_quotes = 0;
	quote_char = 0;
	while (*str)
	{
		if (!in_quotes && (*str == '\'' || *str == '"'))
		{
			quote_char = *str;
			in_quotes = 1;
			str++;
			continue ;
		}
		else if (in_quotes && *str == quote_char)
		{
			in_quotes = 0;
			str++;
			continue ;
		}
		if (!in_quotes && *str == delim)
		{
			count++;
			while (*str == delim)
				str++;
			continue ;
		}
		str++;
	}
	return (count + 1);
}

static char	*get_next_token(char **str, char delim)
{
	const char	*start;
	char		*token;
	int			in_quotes;
	char		quote_char;
	int			len;

	in_quotes = 0;
	quote_char = 0;
	len = 0;
	while (**str && **str == delim)
		(*str)++;
	start = *str;
	while (**str)
	{
		if (!in_quotes && (**str == '\'' || **str == '"'))
		{
			quote_char = **str;
			in_quotes = 1;
			(*str)++;
			len++;
			continue ;
		}
		else if (in_quotes && **str == quote_char)
		{
			in_quotes = 0;
			(*str)++;
			len++;
			continue ;
		}
		if (!in_quotes && **str == delim)
			break ;
		(*str)++;
		len++;
	}
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, len + 1);
	return (token);
}

char	**ft_split_shell(char *str, char delim)
{
	char	**result;
	int		token_count;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	token_count = count_tokens(str, delim);
	result = malloc((token_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (i < token_count)
	{
		result[i] = get_next_token(&str, delim);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**split_with_redirections(char *input)
{
	char	*ptr;
	int		i;
	int		in_quotes;
	char	quote_char;
	char	*start;

	char **tokens = malloc(100 * sizeof(char *)); // Adjust size as needed
	ptr = input;
	i = 0;
	in_quotes = 0;
	quote_char = 0;
	if (!tokens)
		return (NULL);
	while (*ptr)
	{
		// Skip whitespace unless in quotes
		if (!in_quotes && (*ptr == ' ' || *ptr == '\t'))
		{
			ptr++;
			continue ;
		}
		// Handle quotes
		if (*ptr == '\'' || *ptr == '"')
		{
			if (!in_quotes)
			{
				quote_char = *ptr;
				in_quotes = 1;
				ptr++;
				continue ;
			}
			else if (*ptr == quote_char)
			{
				in_quotes = 0;
				ptr++;
				continue ;
			}
		}
		// Handle redirections (only outside quotes)
		if (!in_quotes && (*ptr == '>' || *ptr == '<'))
		{
			if (ptr[1] == *ptr) // >> or <<
			{
				tokens[i++] = ft_strndup(ptr, 2);
				ptr += 2;
			}
			else // > or <
			{
				tokens[i++] = ft_strndup(ptr, 1);
				ptr += 1;
			}
			continue ;
		}
		// Normal token
		start = ptr;
		while (*ptr && (in_quotes || (*ptr != ' ' && *ptr != '\t' && *ptr != '>'
					&& *ptr != '<')))
			ptr++;
		tokens[i++] = ft_strndup(start, ptr - start);
	}
	tokens[i] = NULL;
	return (tokens);
}

char	**remove_null_args(char **args)
{
	int		count;
	int		i;
	char	**new_args;
	int		j;

	count = 0;
	i = 0;
	while (args[count])
		count++;
	new_args = malloc((count + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	j = 0;
	for (i = 0; i < count; i++)
	{
		if (args[i])
		{
			new_args[j] = args[i];
			j++;
		}
	}
	new_args[j] = NULL;
	free(args);
	return (new_args);
}
// int is_builtin( char *cmd)
// {
//     if (!cmd)
//         return (0);

//     static const char *builtins[] = {
//         "cd", "echo", "pwd", "export",
//         "unset", "env", "exit", NULL
//     };

//     for (int i = 0; builtins[i]; i++)
//     {
//         if (ft_strcmp((char *)cmd, (char *)builtins[i]) == 0)
//             return (1);
//     }
//     return (0);
// }
