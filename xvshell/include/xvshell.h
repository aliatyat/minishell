#ifndef XV6SHELL_H
#define XV6SHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <sys/wait.h>

#define MAXARGS 10

// Command types
#define EXEC  ' '   // Execute a command
#define PIPE  '|'   // Pipe `|`
#define REDIR_IN  '<'  // Input redirection `<`
#define REDIR_OUT '>'  // Output redirection `>`

#define WHITESPACE " \t\r\n\v"
#define SYMBOLS "<|>"

// Base command structure
struct cmd {
    int type;  // ' ' (exec), '|' (pipe), '<' or '>' (redirection)
};

// Executable command (`ls`, `wc`, etc.)
struct execcmd {
    int type;               // EXEC (' ')
    char *argv[MAXARGS];    // Arguments to execute
};

// Redirection command (`ls > file`)
struct redircmd {
    int type;        // REDIR_IN or REDIR_OUT
    struct cmd *cmd; // Underlying command
    char *file;      // File for redirection
    int mode;        // File open mode
    int fd;          // File descriptor (0 for `<`, 1 for `>`)
};

// Pipe command (`ls | wc`)
struct pipecmd {
    int type;        // PIPE ('|')
    struct cmd *left;  // Left side of pipe
    struct cmd *right; // Right side of pipe
};

// Function prototypes
int fork1(void);
struct cmd* parsecmd(char*);
void runcmd(struct cmd* cmd);
char* find_command_path(char* cmd);

// Command constructors
struct cmd* execcmd(void);
struct cmd* redircmd(struct cmd *subcmd, char *file, int type);
struct cmd* pipecmd(struct cmd *left, struct cmd *right);

// Parsing functions
struct cmd* parseexec(char **ps, char *es);
struct cmd* parsepipe(char **ps, char *es);
struct cmd* parseline(char **ps, char *es);
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es);
int gettoken(char **ps, char *es, char **q, char **eq);
int peek(char **ps, char *es, char *toks);
char *mkcopy(char *s, char *es);

#endif // XV6SHELL_H
