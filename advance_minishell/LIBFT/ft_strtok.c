#include "libft.h"

char *ft_strtok(char *restrict str, const char *restrict delim)
{
    static char *next;
    char *token;
    //unsigned int i = 0;
    
    if (str)
        next = str;
    
    if (!next)
        return NULL;

    while (*next && ft_strchr(delim, *next))
        next++;
    
    if (!*next)
        return NULL;

    token = next;
    
    while (*next && !ft_strchr(delim, *next))
        next++;

    if (*next)
    {
        *next = '\0';
        next++;
    }
    
    return token;
}
