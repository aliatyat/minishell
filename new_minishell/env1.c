#include "minishell.h"

char **copy_env(char **envp)
{
    char **env;
    int i = 0;

    while (envp[i])
        i++;

    env = malloc((i + 1) * sizeof(char *));
    if (!env)
        return NULL;

    i = 0;
    while (envp[i])
    {
        env[i] = ft_strdup(envp[i]);
        if (!env[i])
        {
            while (i-- > 0)
                free(env[i]);
            free(env);
            return NULL;
        }
        i++;
    }
    env[i] = NULL;
    return env;
}



char *get_env_value(char **env,  char *name)
{
    size_t name_len;
    int i;

    if (!env || !name)
        return NULL;
    
    name_len = ft_strlen(name);
    i = -1;
    while (env[++i])
    {
        if (ft_strncmp(env[i], name, name_len) == 0 && 
            env[i][name_len] == '=')
            return (env[i] + name_len + 1);
    }
    return NULL;
}

char **update_env_var(char **env,  char *name, const char *value)
{
    char *new_var;
    size_t name_len;
    int i;

    if (!env || !name)
        return env;
    
    name_len = ft_strlen(name);
    new_var = ft_strjoin3(name, "=", value);
    if (!new_var)
        return env;
    
    i = -1;
    while (env[++i])
    {
        if (ft_strncmp(env[i], name, name_len) == 0 && 
            env[i][name_len] == '=')
        {
            free(env[i]);
            env[i] = new_var;
            return env;
        }
    }
    
    env = ft_realloc_strarr(env, i + 2);
    if (!env)
    {
        free(new_var);
        return NULL;
    }
    env[i] = new_var;
    env[i + 1] = NULL;
    return env;
}

char **remove_env_var(char **env,  char *var)
{
    int count = 0;
    int i = 0;
    int j = 0;
    char **new_env;
    size_t var_len = ft_strlen(var);

    while (env[count])
        count++;

    new_env = malloc((count + 1) * sizeof(char *));
    if (!new_env)
        return NULL;

    while (env[i])
    {
        if (ft_strncmp(env[i], var, var_len) != 0 || env[i][var_len] != '=')
        {
            new_env[j] = ft_strdup(env[i]);
            if (!new_env[j])
            {
                while (j-- > 0)
                    free(new_env[j]);
                free(new_env);
                return NULL;
            }
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    free_split(env);
    return new_env;
}

void print_sorted_env(char **env, int fd)
{
    int count = 0;
    char **sorted_env;
    int i, j;

    while (env[count])
        count++;

    sorted_env = copy_env(env);
    if (!sorted_env)
        return;

    // Simple bubble sort
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0) {
                char *temp = sorted_env[j];
                sorted_env[j] = sorted_env[j + 1];
                sorted_env[j + 1] = temp;
            }
        }
    }

    i = 0;
    while (sorted_env[i])
    {
        if (ft_strchr(sorted_env[i], '='))
        {
            ft_putstr_fd("declare -x ", fd);
            ft_putendl_fd(sorted_env[i], fd);
        }
        i++;
    }
    close(fd);
    free_split(sorted_env);
}