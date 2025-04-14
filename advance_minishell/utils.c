#include "minishell.h"


// t_command *parse_input(char *input, t_shell *shell)
// {
//     t_command *cmd = NULL;
//     char **tokens;
//     int i = 0;

//     (void)shell; // Mark unused parameter explicitly

//     tokens = ft_split_shell(input, ' '); // Now passing both arguments
//     if (!tokens)
//         return NULL;
    
//     cmd = malloc(sizeof(t_command));
//     if (!cmd)
//     {
//         free_split(tokens);
//         return NULL;
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
//             cmd->out_fd = open(cmd->args[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
//             cmd->args[i] = NULL;
//         }
//         i++;
//     }
    
//     return cmd;
// }





char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *tmp;
    char *result;

    tmp = ft_strjoin(s1, s2);
    if (!tmp)
        return NULL;
    result = ft_strjoin(tmp, s3);
    free(tmp);
    return result;
}

char **ft_realloc_strarr(char **arr, size_t new_size)
{
    char **new;
    size_t i;

    new = malloc(new_size * sizeof(char *));
    if (!new)
        return NULL;
    
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
    
    return new;
}





static int count_tokens(const char *str, char delim)
{
    int count = 0;
    int in_quotes = 0;
    char quote_char = 0;

    while (*str)
    {
        if (!in_quotes && (*str == '\'' || *str == '"'))
        {
            quote_char = *str;
            in_quotes = 1;
            str++;
            continue;
        }
        else if (in_quotes && *str == quote_char)
        {
            in_quotes = 0;
            str++;
            continue;
        }

        if (!in_quotes && *str == delim)
        {
            count++;
            while (*str == delim)
                str++;
            continue;
        }
        str++;
    }
    return (count + 1);
}

static char *get_next_token(const char **str, char delim)
{
    const char *start;
    char *token;
    int in_quotes = 0;
    char quote_char = 0;
    int len = 0;

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
            continue;
        }
        
        else if (in_quotes && **str == quote_char)
        {
            in_quotes = 0;
            (*str)++;
            len++;
            continue;
        }

        if (!in_quotes && **str == delim)
            break;

        (*str)++;
        len++;
    }
    
    token = malloc(len + 1);
    if (!token)
        return NULL;
    
    ft_strlcpy(token, start, len + 1);
    return token;
}

char **ft_split_shell(const char *str, char delim)
{
    char **result;
    int token_count;
    int i = 0;

    if (!str)
        return NULL;
    
    token_count = count_tokens(str, delim);
    result = malloc((token_count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    while (i < token_count)
    {
        result[i] = get_next_token(&str, delim);
        if (!result[i])
        {
            free_split(result);
            return NULL;
        }
        i++;
    }
    result[i] = NULL;
    return result;
}

// int is_builtin( char *cmd)
// {
//     if (!cmd)
//         return 0;
    
//     static const char *builtins[] = {
//         "cd", "echo", "pwd", "export",
//         "unset", "env", "exit", NULL
//     };
    
//     for (int i = 0; builtins[i]; i++)
//     {
//         if (ft_strcmp((char *)cmd, (char *)builtins[i]) == 0)
//             return 1;
//     }
//     return 0;
// }



