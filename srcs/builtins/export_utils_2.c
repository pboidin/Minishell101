/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:30 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:32 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_env(char *const *new_env, int i, t_info *info, char *tmp[2])
{
	if (ft_error_export(new_env[i]) == 0)
		ft_lstadd_back(&info->env,
			ft_lstnew_export(tmp));
}

int	ft_lenvar(const char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

t_env	*ft_lstnew_export(char *tmp[2])
{
	t_env	*newlst;

	newlst = (t_env *)malloc(sizeof(t_env));
	if (!newlst)
		return (free(tmp[0]), free(tmp[1]), NULL);
	newlst->name = tmp[0];
	newlst->value = tmp[1];
	newlst->next = NULL;
	return (newlst);
}

void	ft_update_var(char *var, char *name_var, t_info *info)
{
	char	*content;
	char	*tmp;
	t_env	*env;

	env = info->env;
	tmp = ft_strjoin(name_var, "=");
	free(name_var);
	content = ft_substr(var, ft_lenvar(var) + 1,
			ft_strlen(var) - ft_strlen(tmp));
	while (env != NULL)
	{
		if (ft_strncmp(tmp, (char *)env->value, ft_strlen(tmp)) == 0)
			break ;
		env = env->next;
	}
	free(env->value);
	env->value = ft_strjoin(tmp, content);
	free(tmp);
	free(content);
}
