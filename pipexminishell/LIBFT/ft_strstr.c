#include "libft.h"
char *ft_strstr(const char *haystack, const char *needle)
{
    int i;
    int j;
    
    if (!*needle)
        return (char *)haystack;
    
    i = 0;
    while (haystack[i])
    {
        j = 0;
        while (haystack[i + j] && needle[j] && haystack[i + j] == needle[j])
            j++;
        
        if (!needle[j])
            return (char *)&haystack[i];
        
        i++;
    }
    
    return NULL;
}
