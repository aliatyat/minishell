#include "minishell.h"


#include "minishell.h"

void	init_command(t_command *cmd)
{
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	// cmd->pipefd[0] = -1;
	// cmd->pipefd[1] = -1;
}

void	process_tokens(t_command *cmd, char **tokens, t_shell *shell)
{
	int	i;
	int	arg_pos;

	i = 0;
	arg_pos = 0;
	cmd->args = malloc((count_args(tokens) + 1) * sizeof(char *));
	if (!cmd->args)
		return;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]))
			process_redirection(cmd, tokens, &i);
		else
		{
			cmd->args[arg_pos++] = expand_token(tokens[i], shell);
			i++;
		}
	}
	cmd->args[arg_pos] = NULL;
}

char	*advanced_strtok(char *input)
{
	static char	*ptr;
	char		*start;
	int			in_quotes;
	char		quote_char;

	if (input)
		ptr = input;
	if (!ptr || !*ptr)
		return (NULL);
	/* Tokenization logic with quote and redirection handling */
	/* Similar to previous get_next_token_with_redir */
}


static int	is_redirection(const char *str)
{
	return (ft_strncmp(str, ">>", 2) == 0 || ft_strncmp(str, "<<", 2) == 0 ||
			ft_strncmp(str, ">", 1) == 0 || ft_strncmp(str, "<", 1) == 0);
}

static void	process_redirection(t_command *cmd, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], ">") == 0 && tokens[*i + 1])
	{
		cmd->out_fd = open(tokens[*i + 1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
		(*i) += 2;
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0 && tokens[*i + 1])
	{
		cmd->out_fd = open(tokens[*i + 1], O_WRONLY|O_CREAT|O_APPEND, 0644);
		(*i) += 2;
	}
	else if (ft_strcmp(tokens[*i], "<") == 0 && tokens[*i + 1])
	{
		cmd->in_fd = open(tokens[*i + 1], O_RDONLY);
		(*i) += 2;
	}
}

static char	**split_commands(char *input)
{
	char	**cmds;
	char	*token;
	int		i;

	cmds = malloc(100 * sizeof(char *));
	if (!cmds)
		return (NULL);
	i = 0;
	token = ft_strtok(input, "|");
	while (token)
	{
		cmds[i++] = ft_strtrim(token, " \t\n");
		token = ft_strtok(NULL, "|");
	}
	cmds[i] = NULL;
	return (cmds);
}

static char	**split_tokens(char *input)
{
	char	**tokens;
	char	*token;
	int		i;

	tokens = malloc(100 * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	token = advanced_strtok(input);
	while (token)
	{
		tokens[i++] = ft_strdup(token);
		token = advanced_strtok(NULL);
	}
	tokens[i] = NULL;
	return (tokens);
}

t_command	*unified_tokenizer(char *input, t_shell *shell)
{
	char		**pipe_commands;
	t_command	*head;
	t_command	*current;
	int			i;

	pipe_commands = split_commands(input);
	if (!pipe_commands)
		return (NULL);
	head = NULL;
	i = 0;
	while (pipe_commands[i])
	{
		current = malloc(sizeof(t_command));
		if (!current)
		{
			free_split(pipe_commands);
			free_commands(head);
			return (NULL);
		}
		init_command(current);
		char **tokens = split_tokens(pipe_commands[i]);
		process_tokens(current, tokens, shell);
		free_split(tokens);
		add_command(&head, current);
		i++;
	}
	free_split(pipe_commands);
	return (head);
}