#include "xvshell.h"

char	*find_command_path(char *cmd)
{
	char		*path;
	char		*path_dup;
	char		*token;
	static char	full_path[1024];

	path = getenv("PATH");
	if (!path)
		return (NULL);
	path_dup = strdup(path);
	token = strtok(path_dup, ":");
	while (token)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_dup);
			return (full_path);
		}
		token = strtok(NULL, ":");
	}
	free(path_dup);
	return (NULL);
}

void	runcmd(struct cmd *cmd)
{
	int				fd_redirect;
	int				p_id;
	int				fd_pipe[2];
	struct execcmd	*ecmd;
	struct pipecmd	*pcmd;
	struct redircmd	*rcmd;
	char			*full_path;

	if (cmd == NULL)
		exit(0);
	if (cmd->type == '>')
	{
		rcmd = (struct redircmd *)cmd;
		if ((fd_redirect = open(rcmd->file, rcmd->mode, 0666)) < 0)
		{
			fprintf(stderr, "open %s failed\n", rcmd->file);
			exit(1);
		}
		if (dup2(fd_redirect, rcmd->fd) < 0)
		{
			fprintf(stderr, "dup2 failed\n");
			exit(1);
		}
		close(fd_redirect);
		if (rcmd->cmd->type == ' ')
			runcmd(rcmd->cmd);
		else
		{
			fprintf(stderr, "Error Redirection must followed command.\n");
			exit (1);
		}
	}
	else if (cmd->type == '|')
	{
		pcmd = (struct pipecmd *)cmd;
		if (pipe(fd_pipe) < 0)
		{
			fprintf(stderr, "pipe failed\n");
			exit(1);
		}
		if ((p_id = fork()) < 0)
		{
			fprintf(stderr, "fork failed\n");
			exit(1);
		}
		if (p_id == 0)
		{
			close(fd_pipe[1]);
			dup2(fd_pipe[0], STDIN_FILENO);
			close(fd_pipe[0]);
			if (pcmd->right)
				runcmd(pcmd->right);
		}
		else
		{
			close(fd_pipe[0]);
			dup2(fd_pipe[1], STDOUT_FILENO);
			close(fd_pipe[1]);
			if (pcmd->left)
				runcmd(pcmd->left);
			wait(NULL);
		}
	}
	else if (cmd->type == ' ')
	{
		ecmd = (struct execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			exit(0);
		full_path = find_command_path(ecmd->argv[0]);
		if (!full_path)
		{
			fprintf(stderr, "%s: command not found\n", ecmd->argv[0]);
			exit(1);
		}
		execve(full_path, ecmd->argv, NULL);
		perror("execve failed");
		exit(1);
	}
	else
	{
		fprintf(stderr, "Unknown command type: %d\n", cmd->type);
		exit(1);
	}
}
