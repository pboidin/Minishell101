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
	{
		free(aux);
		return (1);
	}
	aux = ft_realloc(aux, ft_strlen(dir) + ft_strlen(home) + 1);
	tmp = ft_strjoin(home, aux);
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
		path = ft_genv("PWD", env);
		if (!path)
			return ;
	}
	free(env->value);
	printf("Path %s\n", path);
	env->value = ft_strjoin("PWD=", path);
	free(path);
}

static int	ft_go_to_oldpwd(t_env *env)
{
	char	*tmp;
	char	*old;
	char	*pwd;

	tmp = NULL;
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
	ft_set_old(env, pwd, tmp);
	ft_env_loc(env);
	return (0);
}

int	ft_ch_dir(char **dir, t_info *info)
{
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	tmp = NULL;
	if (dir[1] && ft_strncmp(dir[1], "-", 1) == 0)
		return (ft_go_to_oldpwd(info->env));
	ft_set_old(info->env, pwd, tmp);
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
