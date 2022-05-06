/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:48:06 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:48:09 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int unset_env(char *const *unset, int i, t_info *info)
{
	t_env	*tmp_env;
	t_env	*current_env;

	current_env = info->env;
	if (current_env && ft_strcmp(unset[i], info->env->name) == 0)
	{
		info->env = current_env->next;
		return (free(current_env->name), free(current_env->value), \
		free(current_env), 1);
	}
	while (current_env && current_env->next
		&& ft_strcmp(current_env->next->name, unset[i]) != 0)
		current_env = current_env->next;
	if (current_env && current_env->next)
	{
		tmp_env = current_env->next;
		current_env->next = current_env->next->next;
		return (free(tmp_env->name), free(tmp_env->value), free(tmp_env), 1);
	}
	return (0);
}

void	unset_lcl(char *const *unset, int i, t_info *info)
{
	t_var	*tmp_lcl;
	t_var	*current_lcl;
	
	current_lcl = info->local_var;
	if (current_lcl && ft_strcmp(unset[i], info->local_var->name) == 0)
	{
		info->local_var = current_lcl->next;
		free(current_lcl->name);
		free(current_lcl->value);
		free(current_lcl);
		return ;
	}
	while (current_lcl && current_lcl->next
		&& ft_strcmp(current_lcl->next->name, unset[i]) != 0)
		current_lcl = current_lcl->next;
	if (current_lcl && current_lcl->next)
	{
		tmp_lcl = current_lcl->next;
		current_lcl->next = current_lcl->next->next;
		free(tmp_lcl->name);
		free(tmp_lcl->value);
		free(tmp_lcl);
	}
}

void	ft_unset_exec(char *const *unset, int i, t_info *info)
{
	if (!unset_env(unset, i, info))
		unset_lcl(unset, i, info);
}

int	ft_unset(char **unset, t_info *info)
{
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	if (unset[1])
	{
		while (unset[++i])
		{
			if (ft_unset_handle(unset[i]) == 0)
				ft_unset_exec(unset, i, info);
			else
				ret = 1;
		}
	}
	return (ret);
}
