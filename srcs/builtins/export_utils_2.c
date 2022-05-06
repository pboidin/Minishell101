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