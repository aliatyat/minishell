#include "minishell.h"



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

// char	**remove_null_args(char **args)
// {
// 	int		count;
// 	int		i;
// 	char	**new_args;
// 	int		j;

// 	count = 0;
// 	i = 0;
// 	while (args[count])
// 		count++;
// 	new_args = malloc((count + 1) * sizeof(char *));
// 	if (!new_args)
// 		return (NULL);
// 	j = 0;
// 	for (i = 0; i < count; i++)
// 	{
// 		if (args[i])
// 		{
// 			new_args[j] = args[i];
// 			j++;
// 		}
// 	}
// 	new_args[j] = NULL;
// 	free(args);
// 	return (new_args);
// }

char	**split_with_redirections(char *input)
{
	char	*ptr;
	int		i;
	int		in_quotes;
	//char	quote_char;
	char	*start;
	printf("split\n");
	char **tokens = malloc(100 * sizeof(char *)); // Adjust size as needed
	ptr = input;
	i = 0;
	in_quotes = 0;
	//quote_char = 0;
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
		// if (*ptr == '\'' || *ptr == '"')
		// {
		// 	if (!in_quotes)
		// 	{
		// 		quote_char = *ptr;
		// 		in_quotes = 1;
		// 		ptr++;
		// 		continue ;
		// 	}
		// 	else if (*ptr == quote_char)
		// 	{
		// 		in_quotes = 0;
		// 		ptr++;
		// 		continue ;
		// 	}
		// }
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
				printf("split\n");
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




char	**split_ex(char *input)
{
	char	*ptr;
	int		i;
	int		in_quotes;
	//char	quote_char;
	char	*start;
	printf("split\n");
	char **tokens = malloc(100 * sizeof(char *)); // Adjust size as needed
	ptr = input;
	i = 0;
	in_quotes = 0;
	//quote_char = 0;
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
		// if (*ptr == '\'' || *ptr == '"')
		// {
		// 	if (!in_quotes)
		// 	{
		// 		quote_char = *ptr;
		// 		in_quotes = 1;
		// 		ptr++;
		// 		continue ;
		// 	}
		// 	else if (*ptr == quote_char)
		// 	{
		// 		in_quotes = 0;
		// 		ptr++;
		// 		continue ;
		// 	}
		// }
		// Handle redirections (only outside quotes)
		if (!in_quotes && (*ptr == '$'))
		{
			if (ptr[1] == *ptr) // >> or <<
			{
				tokens[i++] = ft_strndup(ptr, 2);
				ptr += 2;
			}
			else // > or <
			{
				printf("split\n");
				tokens[i++] = ft_strndup(ptr, 1);
				ptr += 1;
			}
			continue ;
		}
		// Normal token
		start = ptr;
		while (*ptr && (in_quotes || (*ptr != ' ' && *ptr != '\t' && *ptr != '$')))
			ptr++;
		tokens[i++] = ft_strndup(start, ptr - start);
	}
	tokens[i] = NULL;
	return (tokens);
}
// t_command	*create_command(char *input)
// {
// 	t_command	*cmd;
// 	char		**tokens;
// 	int			i;

// 	i = 0;
// 	//char *ptr = input;
// 	cmd = malloc(sizeof(t_command));
// 	if (!cmd)
// 		return (NULL);
// 	ft_memset(cmd, 0, sizeof(t_command));
// 	cmd->in_fd = STDIN_FILENO;
// 	cmd->out_fd = STDOUT_FILENO;
// 	// cmd->pipefd[0] = -1;
// 	// cmd->pipefd[1] = -1;
// 	// First pass: split including redirection symbols as separate tokens
// 	tokens = split_with_redirections(input);
// 	if (!tokens)
// 	{
// 		free(cmd);
// 		return (NULL);
// 	}
// 	// Second pass: process redirections
// 	i = 0;
// 	while (tokens[i])
// 	{
// 		if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
// 		{
// 			cmd->out_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
// 					0644);
// 			free(tokens[i]);
// 			free(tokens[i + 1]);
// 			tokens[i] = NULL;
// 			tokens[i + 1] = NULL;
// 			i += 2;
// 		}
// 		else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
// 		{
// 			cmd->out_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND,
// 					0644);
// 			free(tokens[i]);
// 			free(tokens[i + 1]);
// 			tokens[i] = NULL;
// 			tokens[i + 1] = NULL;
// 			i += 2;
// 		}
// 		else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
// 		{
// 			cmd->in_fd = open(tokens[i + 1], O_RDONLY);
// 			free(tokens[i]);
// 			free(tokens[i + 1]);
// 			tokens[i] = NULL;
// 			tokens[i + 1] = NULL;
// 			i += 2;
// 		}
// 		else
// 		{
// 			i++;
// 		}
// 	}
// 	// Compact the arguments array (remove NULLs)
// 	cmd->args = remove_null_args(tokens);
// 	return (cmd);
// }

