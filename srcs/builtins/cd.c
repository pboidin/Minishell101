/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:45:58 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/10 19:52:22 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_go_to_dir(const char *dir, t_info *info)
{
	char	*tmp;
	char	*aux;
	char	*home;

	aux = ft_substr(dir, 1, ft_strlen(dir) - 1);
	home = ft_genv("HOME", info->env);
	if (!home)
		return (free(aux), 1);
	aux = ft_realloc(aux, ft_strlen(dir) + ft_strlen(home) + 1);
	tmp = ft_strjoin(home, aux);
	printf("%s\n", tmp);
	if (chdir(tmp) == -1)
	{
		ft_print_err_cd(tmp);
		free(home);
		free(aux);
		free(tmp);
		return (1);
	}
	free(home);
	free(aux);
	free(tmp);
	return (0);
}

void	ft_upd_env(t_env *env)
{
	char	*path;

	path = malloc(sizeof(char) * (PATH_MAX + 1));
	if (!path)
		return ;
	if (getcwd(path, PATH_MAX) == NULL)
	{
		free(path);
		return ;
	}
	free(env->value);
	env->value = path;
}

static int	ft_go_to_oldpwd(t_env *env)
{
	char	*old;
	char	*pwd;

	pwd = NULL;
	old = ft_genv("OLDPWD", env);
	if (!old)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return (1);
	}
	if (chdir(old) == -1)
	{
		write(STDERR_FILENO, "cd: ", 4);
		write(STDERR_FILENO, old, ft_strlen(old));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		free(old);
		return (1);
	}
	free(old);
	ft_set_old(env, pwd);
	ft_env_loc(env);
	return (0);
}

int	array_len(char **dir)
{
	int	i;

	i = 0;
	while (dir[i])
		i++;
	return (i);
}

int	ft_ch_dir(char **dir, t_info *info)
{
	char	*pwd;

	pwd = NULL;
	if (array_len(dir) > 2)
		return (write(STDERR_FILENO, "cd: too many arguments\n", 23), 1);
	if (dir[1] && ft_strncmp(dir[1], "-", 1) == 0)
		return (ft_go_to_oldpwd(info->env));
	ft_set_old(info->env, pwd);
	if (!dir[1] || (!ft_strncmp(dir[1], "~\0", 2)
			|| !ft_strncmp(dir[1], "~/", 2)))
	{
		if (!dir[1] || !ft_strncmp(dir[1], "~\0", 2))
		{
			if (ft_ret_home(info) == 1)
				return (1);
			return (0);
		}
		else if (ft_go_to_dir(dir[1], info) == 1)
			return (1);
	}
	else if (dir[1][0] != '\0' && chdir(dir[1]) == -1)
		return (ft_print_err_cd(dir[1]));
	else
		ft_env_set(info->env);
	return (0);
}
