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

static void	ft_delete_join(t_env *aux, int c, t_info *info)
{
	t_env	*tmp;

	tmp = NULL;
	if (c == 0)
	{
		aux = info->env;
		info->env = (info->env)->next;
		ft_lstdelone(aux, free);
		return ;
	}
	if (aux->next && aux->next->next)
		tmp = aux->next->next;
	ft_lstdelone(aux->next, free);
	aux->next = tmp;
}

static t_env	*ft_build_str(const char *unset, char **tmp, t_info *info)
{
	char	*tmp1;
	t_env	*aux;

	ft_strlcpy(*tmp, unset, ft_strlen(unset) + 1);
	tmp1 = *tmp;
	*tmp = ft_strjoin(tmp1, "=");
	free(tmp1);
	aux = info->env;
	return (aux);
}

static t_env	*ft_find_pos(const char *unset, int *c, char *tmp, t_info *info)
{
	t_env	*aux;

	if (unset == NULL)
		return (NULL);
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(unset) + 1));
	if (!tmp)
		return (NULL);
	aux = ft_build_str(unset, &tmp, info);
	if (ft_strncmp((char *)aux->next->value, tmp, ft_strlen(tmp)) == 0)
	{
		free (tmp);
		return (aux);
	}
	while (aux && aux->next)
	{
		(*c)++;
		if (ft_strncmp((char *)aux->next->value, tmp, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			return (aux);
		}
		aux = aux->next;
	}
	free(tmp);
	return (NULL);
}

void	ft_unset_exec(char *const *unset, t_env *aux, int i, t_info *info)
{
	int		j;
	char	*tmp;

	j = 0;
	tmp = NULL;
	aux = ft_find_pos(unset[i], &i, tmp, info);
	if (aux != NULL)
		ft_delete_join(aux, j, info);
}

int	ft_unset(char **unset, t_info *info)
{
	int		i;
	int		ret;
	t_env	*aux;

	aux = info->env;
	i = 0;
	ret = 0;
	if (unset[1] && !aux)
		return (0);
	else if (unset[1] && aux)
	{
		while (unset[++i])
		{
			if (ft_unset_handle(unset[i]) == 0)
				ft_unset_exec(unset, aux, i, info);
			else
				ret = 1;
		}
	}
	return (ret);
}
