/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:46:39 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/05 07:39:11 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_go_to_home(t_info *info)
{
	char	*home;

	home = ft_genv("HOME", info);
	if (!home || chdir(home) == -1)
	{
		ft_print_err_cd(home);
		free(home);
		return (0);
	}
	ft_env_loc(info);
	free(home);
	return (0);
}

int	ft_env_loc(t_info *info)
{
	t_env	*aux;

	aux = info->env;
	while (aux != NULL)
	{
		if (ft_strncmp("PWD=", (char *)aux->value, 4) == 0)
		{
			ft_upd_env(info);
			return (0);
		}
		aux = aux->next;
	}
	return (1);
}

void	ft_set_val(t_info *info, const char *val, int c)
{
	if (c == 0)
	{
		free(info->env->value);
		info->env->value = (void *)ft_strdup(val);
	}
	else
		ft_lstadd_back(&info->env, ft_lstnew((void *) ft_strdup(val)));
}

int	ft_try_go_oldpwd(t_env **env)
{
	t_env	*aux;

	aux = *env;
	if (!*env)
		return (1);
	while (*env)
	{
		if (ft_strncmp("OLDPWD=", (char *)(*env)->value, 7) == 0)
			return (0);
		*env = (*env)->next;
	}
	*env = aux;
	return (1);
}

int	ft_set_old(t_info *info, char *pwd, char *val)
{
	int		ret;
	t_env	*tmp;

	tmp = info->env;
	pwd = ft_genv("PWD", info);
	ret = ft_try_go_oldpwd(&tmp);
	if (tmp == NULL)
	{
		free(pwd);
		return (1);
	}
	if (pwd == NULL)
	{
		pwd = (char *)malloc(sizeof(char) * (PATH_MAX + 1));
		if (!pwd || getcwd(pwd, PATH_MAX) == NULL)
		{
			free(pwd);
			return (1);
		}
	}
	val = ft_strjoin("OLDPWD=", pwd);
	ft_set_val(info, val, ret);
	free(pwd);
	free(val);
	return (0);
}
