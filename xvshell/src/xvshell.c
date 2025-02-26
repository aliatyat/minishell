#include "xvshell.h"


int	main(void)
{
	static char	*line;

	int	r;
	while ((line = readline("\033[1;32m$>\033[0m ")) != (void *)0)
	{
		if (line[0] == 'c' && line[1] == 'd' && line[2] == ' ')
		{
			line[strlen(line) - 1] = 0;
			if (chdir(line + 3) < 0)
				fprintf(stderr, "cannot cd %s\n", line + 3);
			continue ;
		}
		if (fork1() == 0)
			runcmd(parsecmd(line));
		wait(&r);
	}
	exit(0);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return (pid);
}
