#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "minishell.h"


static char	*strndup_wrapper(const char *s, int n)
{
	char	*ret = malloc(n + 1);
	if (!ret)
		return (NULL);
	strncpy(ret, s, n);
	ret[n] = '\0';
	return (ret);
}

char	**split_tokens(const char *input)
{
	char	**tokens = malloc(sizeof(char *) * 1024);
	int		i = 0, j = 0, k = 0;
	char	quote;

	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			j = 1;
			while (input[i + j] && input[i + j] != quote)
				j++;
			if (input[i + j] == quote)
				j++; // include closing quote
			tokens[k++] = strndup_wrapper(&input[i], j);
			i += j;
		}
		else if (!strncmp(&input[i], ">>", 2) || !strncmp(&input[i], "<<", 2))
		{
			tokens[k++] = strndup_wrapper(&input[i], 2);
			i += 2;
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			tokens[k++] = strndup_wrapper(&input[i], 1);
			i++;
		}
		else
		{
			j = 0;
			while (input[i + j]
				&& !isspace(input[i + j])
				&& input[i + j] != '\'' && input[i + j] != '"'
				&& strncmp(&input[i + j], ">>", 2)
				&& strncmp(&input[i + j], "<<", 2)
				&& input[i + j] != '>' && input[i + j] != '<'
				&& input[i + j] != '|')
				j++;
			tokens[k++] = strndup_wrapper(&input[i], j);
			i += j;
		}
	}
	tokens[k] = NULL;
	return (tokens);
}
static int	is_redirection(const char *s)
{
	return (!strcmp(s, ">") || !strcmp(s, ">>")
		|| !strcmp(s, "<") || !strcmp(s, "<<")
		|| !strcmp(s, ">&") || !strcmp(s, "<&"));
}

static int	is_pipe(const char *s)
{
	return (!strcmp(s, "|"));
}

static int	count_tokens(char **tokens)
{
	int	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

static char	**normalize_segment(char **tokens, int start, int end)
{
	char	**norm = malloc(sizeof(char *) * (end - start + 10));
	char	**args = malloc(sizeof(char *) * (end - start + 10));
	char	**redir = malloc(sizeof(char *) * (end - start + 10));
	char	*cmd = NULL;
	int		i = start, a = 0, r = 0, ni = 0;

	while (i < end)
	{
		if (is_redirection(tokens[i]) && (i + 1 < end))
		{
			redir[r++] = tokens[i++];
			redir[r++] = tokens[i];
		}
		else if (!cmd && !is_pipe(tokens[i]))
			cmd = tokens[i];
		else if (!is_pipe(tokens[i]))
			args[a++] = tokens[i];
		i++;
	}
if (!cmd)
{
	// No command: return original range (no normalization)
	char **raw = malloc(sizeof(char *) * (end - start + 1));
	int x = 0;
	for (int i = start; i < end; i++)
		raw[x++] = tokens[i];
	raw[x] = NULL;
	free(args);
	free(redir);
	free(norm);
	return (raw);
}

	norm[ni++] = cmd;
	for (i = 0; i < a; i++)
		norm[ni++] = args[i];
	for (i = 0; i < r; i++)
		norm[ni++] = redir[i];
	norm[ni] = NULL;
	free(args);
	free(redir);
	return (norm);
}

char	**normalize_command(char **tokens)
{
	int		total = count_tokens(tokens);
	char	**result = malloc(sizeof(char *) * (total + 10));
	int		i = 0, start = 0, ri = 0;

	while (i <= total)
	{
		if (!tokens[i] || is_pipe(tokens[i]))
		{
			char **segment = normalize_segment(tokens, start, i);
			int j = 0;
			while (segment[j])
				result[ri++] = segment[j++];
			free(segment); // we only free array, not contents (shared)
			if (tokens[i])
				result[ri++] = tokens[i]; // copy the pipe token "|"
			start = i + 1;
		}
		i++;
	}
	result[ri] = NULL;
	return (result);
}
 

static int	calc_total_len(char **tokens)
{
	int	i = 0;
	int	len = 0;

	while (tokens[i])
		len += strlen(tokens[i++]) + 1; // +1 for space
	return (len);
}

char	*join_tokens_back(char **tokens)
{
	int		total_len = calc_total_len(tokens);
	char	*joined = malloc(total_len);
	int		i = 0;
	int		pos = 0;

	if (!joined)
		return (NULL);
	while (tokens[i])
	{
		int len = strlen(tokens[i]);
		memcpy(joined + pos, tokens[i], len);
		pos += len;
		if (((tokens[i + 1]) && (ft_strcmp(tokens[i], "<<") != 0)) && (ft_strcmp(tokens[i], ">") != 0) && (ft_strcmp(tokens[i], "<") != 0)
			&& (ft_strcmp(tokens[i], ">>") != 0))
			joined[pos++] = ' ';
		i++;
	}
	joined[pos] = '\0';
	return (joined);
}
/*
int	main()
{
	while (1)
	{
		char *str = readline("minishell$> ");
		if (!str)
			break;
		char **arr = split_tokens(str);
		int i = 0;
		while (arr[i] != 0)
			printf("split %s\n", arr[i++]);
		char	**norm = normalize_command(arr);
		i = 0;
		while (norm[i] != 0)
			printf("norm %s\n", norm[i++]);
		char *normback = join_tokens_back(norm);
		printf("join back [%s]\n", normback);
		printf("\n");
	}
	return (0);
}


char **input1 = split_tokens(input);
	int x = 0;
	printf("gggggggggggggggsss %s\n", input1[0]);
	pipe_commands = normalize_command(input1);
	printf("fffffffffffffffff %s %s\n", pipe_commands[0] , pipe_commands[1]);
	input = join_tokens_back(pipe_commands);
	printf("INPUT2 %s\n", input);
	pipe_commands = ft_split_pipes(input);
*/

/*
char **input1 = split_tokens(input);
	int x = 0;
	printf("gggggggggggggggsss %s\n", input1[0]);
	pipe_commands = normalize_command(input1);
	printf("fffffffffffffffff %s %s\n", pipe_commands[0] , pipe_commands[1]);
	input = join_tokens_back(pipe_commands);
	printf("INPUT2 %s\n", input);
	pipe_commands = ft_split_pipes(input);
*/
