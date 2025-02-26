#include "xvshell.h"

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && strchr(WHITESPACE, *s))
		s++;
	if (q)
		*q = s;
	ret = *s;
	if (*s == 0)
	{
	}
	else if (*s == '|')
	{
		s++;
	}
	else if (*s == '<')
	{
		s++;
	}
	else if (*s == '>')
	{
		s++;
	}
	else
	{
		ret = 'a';
		while (s < es && !strchr(WHITESPACE, *s) && !strchr(SYMBOLS, *s))
			s++;
	}
	if (eq)
		*eq = s;
	while (s < es && strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (ret);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && strchr(WHITESPACE, *s))
		s++;
	*ps = s;
	return (*s && strchr(toks, *s));
}

char	*mkcopy(char *s, char *es)
{
	int		n;
	char	*c;

	n = es - s;
	c = malloc(n + 1);
	assert(c);
	strncpy(c, s, n);
	c[n] = 0;
	return (c);
}
