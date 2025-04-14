#include "minishell.h"

char **ft_split_pipes(char *input)
{
    char **cmds;
    int i = 0;
    char *token;
    char *input_copy;

    if (!input)
        return NULL;

    input_copy = ft_strdup(input);
    if (!input_copy)
        return NULL;

    cmds = malloc(100 * sizeof(char *));
    if (!cmds)
    {
        free(input_copy);
        return NULL;
    }

    token = ft_strtok(input_copy, "|");
    while (token)
    {
        char *trimmed = ft_strtrim(token, " \t\n");
        if (!trimmed)
        {
            free_2d_array(cmds);
            free(input_copy);
            return NULL;
        }
        
        cmds[i] = trimmed;
        token = ft_strtok(NULL, "|");
        i++;
        
        if (i >= 99) // Prevent buffer overflow
            break;
    }
    cmds[i] = NULL;
    free(input_copy);
    return cmds;
}