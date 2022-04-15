/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:46:39 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/10 19:48:58 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_go_to_home(t_info *info)
{
	char	*home;
	t_env	*head;

	home = ft_genv("HOME", info->env);
	head = info->env;
	if (!home || chdir(home) == -1)
	{
		ft_print_err_cd(home);
		free(home);
		return (0);
	}
	ft_env_loc(head);
	free(home);
	return (0);
}

int	ft_env_loc(t_env *head)
{
	t_env	*aux;

	aux = head;
	while (aux != NULL)
	{
		if (ft_strcmp("PWD", (char *)aux->name) == 0)
		{
			ft_upd_env(aux);
			return (0);
		}
		aux = aux->next;
	}
	return (1);
}

void	ft_set_val(t_env **env, char *val, int c)
{
	if (c == 0)
	{
		free((*env)->value);
		(*env)->value = val;
	}
	else
		ft_lstadd_back(env, ft_lstnew((void *) ft_strdup(val)));
}

int	ft_try_go_oldpwd(t_env **env)
{
	if (!*env)
		return (1);
	while (*env)
	{
		if (ft_strcmp("OLDPWD", (char *)(*env)->name) == 0)
			return (0);
		*env = (*env)->next;
	}
	return (1);
}

int	ft_set_old(t_env *env, char *pwd)
{
	int		ret;
	t_env	*tmp;

	pwd = ft_genv("PWD", env);
	tmp = env;
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
	ft_set_val(&tmp, pwd, ret);
	return (0);
}
