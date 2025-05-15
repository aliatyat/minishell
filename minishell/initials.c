/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initials.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:45:18 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/13 20:09:25 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_shell(t_shell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env = copy_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: environment copy failed", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	set_initial_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		shell->env = update_env_var(shell->env, "PWD", cwd);
	return (1);
}

int	check_lvl(t_shell *shell)
{
	char	*env_val;
	int		f_atoi;
	char	*f_itoa;

	env_val = get_env_value(shell->env, "SHLVL");
	f_atoi = ft_atoi(env_val) + 1;
	f_itoa = ft_itoa(f_atoi);
	if (f_atoi > 9)
		update_env_var(shell->env, "SHLVL", "1");
	else
		update_env_var(shell->env, "SHLVL", f_itoa);
	free(f_itoa);
	free(env_val);
	return (0);
}
