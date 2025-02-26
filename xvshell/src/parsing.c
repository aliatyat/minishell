#include "xvshell.h"

struct cmd	*parsecmd(char *s)
{
	char		*es;
	struct cmd	*cmd;

	es = s + strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		fprintf(stderr, "leftovers: %s\n", s);
		exit(-1);
	}
	return (cmd);
}

struct cmd	*parseline(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parsepipe(ps, es);
	return (cmd);
}

struct cmd	*parsepipe(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}
struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es)
{
	int	tok;

	char *q, *eq;
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			fprintf(stderr, "missing file for redirection\n");
			exit(-1);
		}
		if (tok == '<')
		{
			cmd = redircmd(cmd, mkcopy(q, eq), '<');
		}
		else if (tok == '>')
		{
			cmd = redircmd(cmd, mkcopy(q, eq), '>');
		}
	}
	return (cmd);
}

struct cmd	*parseexec(char **ps, char *es)
{
	struct execcmd	*cmd;
	struct cmd		*ret;

	char *q, *eq;
	int tok, argc;
	ret = execcmd();
	cmd = (struct execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			fprintf(stderr, "syntax error\n");
			exit(-1);
		}
		cmd->argv[argc] = mkcopy(q, eq);
		argc++;
		if (argc >= MAXARGS)
		{
			fprintf(stderr, "too many args\n");
			exit(-1);
		}
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	return (ret);
}
